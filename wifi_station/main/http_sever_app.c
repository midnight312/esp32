/* Simple HTTP Server Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <sys/param.h>
#include "esp_netif.h"
#include "esp_eth.h"

#include "esp_tls_crypto.h"
#include <esp_http_server.h>


/* A simple example that demonstrates how to create GET and POST
 * handlers for the web server.
 */

static httpd_handle_t server = NULL;
extern const char index_html_start[] asm("_binary_anh_jpeg_start");
extern const char index_html_end[] asm("_binary_anh_jpeg_end");

/* An HTTP GET handler */
static esp_err_t hello_get_handler(httpd_req_t *req)
{

    const char* resp_str = (const char*) "hello word";
    httpd_resp_send(req, resp_str,HTTPD_RESP_USE_STRLEN);
    //httpd_resp_set_type(req,"image/jpeg");
    //httpd_resp_send(req, index_html_start, index_html_end - index_html_start);
    return ESP_OK;
}

static const httpd_uri_t http_get_dht11 = {
    .uri       = "/dht11",
    .method    = HTTP_GET,
    .handler   = hello_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = "Hello World!"
};

static esp_err_t data_post_handler(httpd_req_t *req)
{

    char buf[100];
    httpd_req_recv(req, buf,req->content_len);
    printf("DATA: %s\n",buf);
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t post_data = {
    .uri       = "/data",
    .method    = HTTP_POST,
    .handler   = data_post_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = NULL
};

// /* An HTTP POST handler */
// static esp_err_t echo_post_handler(httpd_req_t *req)
// {
//     char buf[100];
//     int ret, remaining = req->content_len;

//     while (remaining > 0) {
//         /* Read the data for the request */
//         if ((ret = httpd_req_recv(req, buf,
//                         MIN(remaining, sizeof(buf)))) <= 0) {
//             if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
//                 /* Retry receiving if timeout occurred */
//                 continue;
//             }
//             return ESP_FAIL;
//         }

//         /* Send back the same data */
//         httpd_resp_send_chunk(req, buf, ret);
//         remaining -= ret;

//         /* Log data received */
//         ESP_LOGI(TAG, "=========== RECEIVED DATA ==========");
//         ESP_LOGI(TAG, "%.*s", ret, buf);
//         ESP_LOGI(TAG, "====================================");
//     }

//     // End response
//     httpd_resp_send_chunk(req, NULL, 0);
//     return ESP_OK;
// }

// static const httpd_uri_t echo = {
//     .uri       = "/echo",
//     .method    = HTTP_POST,
//     .handler   = echo_post_handler,
//     .user_ctx  = NULL
// };

/* This handler allows the custom error handling functionality to be
 * tested from client side. For that, when a PUT request 0 is sent to
 * URI /ctrl, the /hello and /echo URIs are unregistered and following
 * custom error handler http_404_error_handler() is registered.
 * Afterwards, when /hello or /echo is requested, this custom error
 * handler is invoked which, after sending an error message to client,
 * either closes the underlying socket (when requested URI is /echo)
 * or keeps it open (when requested URI is /hello). This allows the
 * client to infer if the custom error handler is functioning as expected
 * by observing the socket state.
 */
esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    if (strcmp("/hello", req->uri) == 0) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/hello URI is not available");
        /* Return ESP_OK to keep underlying socket open */
        return ESP_OK;
    } 
    
    /* For any other URI send 404 and close socket */
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Some 404 error message");
    return ESP_FAIL;
}


 void start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    //ESP_LOGI(tag, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        //ESP_LOGI(tag, "Registering URI handlers");
        httpd_register_uri_handler(server, &post_data);
        httpd_register_uri_handler(server, &http_get_dht11);
        
        httpd_register_err_handler(server, HTTPD_404_NOT_FOUND, http_404_error_handler);
        //httpd_register_uri_handler(server, &echo);
        //httpd_register_uri_handler(server, &ctrl);
    }

    //ESP_LOGI(tag, "Error starting server!");
}

void stop_webserver(void)
{
    // Stop the httpd server
    httpd_stop(server);
}





