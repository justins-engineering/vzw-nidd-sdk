#include <stdlib.h>
#include <string.h>
#include <vzw/registered_callback_listeners.h>

#include "../curl/helpers.h"
#include "../log.h"
#include "api_fields.h"

static CURL *registered_callback_listeners(
    const char *account_name, char *auth_token, char *session_token, void *header_data,
    CurlRecvData *response_data, struct curl_slist *headers
) {
  char *ptr;
  CURL *curl = curl_easy_init();

  char url[VZW_CALLBACKS_API_LEN(account_name)];
  char access_token_field[ACCESS_TOKEN_FIELD_SIZE(auth_token)];
  char session_token_field[SESSION_TOKEN_FIELD_SIZE(session_token)];

  ptr = stpcpy(url, VZW_CALLBACKS_API);
  (void)stpcpy(ptr, account_name);

  ptr = stpcpy(access_token_field, ACCESS_TOKEN_FIELD);
  (void)stpcpy(ptr, auth_token);

  ptr = stpcpy(session_token_field, SESSION_TOKEN_FIELD);
  (void)stpcpy(ptr, session_token);

  headers = curl_slist_append(headers, CONTENT_TYPE_JSON);
  headers = curl_slist_append(headers, ACCEPT_JSON);
  headers = curl_slist_append(headers, access_token_field);
  headers = curl_slist_append(headers, session_token_field);

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_default_opts(curl, header_data, response_data, headers);

  return curl;
}

int vzw_get_registered_callback_listeners(
    const char *account_name, char *auth_token, char *session_token, void *response_data
) {
  CURLcode res;
  struct curl_slist *headers = NULL;
  CurlRecvData header_data = {NULL, 0};

  CURL *curl = registered_callback_listeners(
      account_name, auth_token, session_token, &header_data, response_data, headers
  );

  res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    PRINTERR("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    goto fail;
  }

fail:
  free(header_data.response);
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);

  return res;
}

int vzw_set_registered_callback_listeners(
    const char *account_name, char *auth_token, char *session_token, void *response_data,
    char *service_name, char *url
) {
  char *ptr;
  CURLcode res;
  struct curl_slist *headers = NULL;
  CurlRecvData header_data = {NULL, 0};

  char post_field[sizeof("{\"name\":\"\",\"url\":\"\"}") + strlen(service_name) + strlen(url)];

  CURL *curl = registered_callback_listeners(
      account_name, auth_token, session_token, &header_data, response_data, headers
  );

  post_field[0] = '{';
  ptr = stpcpy(&post_field[1], "\"name\":\"");
  ptr = stpcpy(ptr, service_name);
  ptr = stpcpy(ptr, "\",");
  ptr = stpcpy(ptr, "\"url\":\"");
  ptr = stpcpy(ptr, url);
  (void)stpcpy(ptr, "\"}");

  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_field);

  res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    PRINTERR("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    goto fail;
  }

fail:
  free(header_data.response);
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);

  return res;
}

int vzw_delete_registered_callback_listeners(
    const char *account_name, char *auth_token, char *session_token, void *response_data,
    char *service_name
) {
  char *ptr;
  CURLcode res;
  struct curl_slist *headers = NULL;
  CurlRecvData header_data = {NULL, 0};

  char url[VZW_CALLBACKS_API_LEN(account_name) + sizeof("/name/") + strlen(service_name) - 1];

  ptr = stpcpy(url, VZW_CALLBACKS_API);
  ptr = stpcpy(ptr, account_name);
  ptr = stpcpy(ptr, "/name/");
  (void)stpcpy(ptr, service_name);

  CURL *curl = registered_callback_listeners(
      account_name, auth_token, session_token, &header_data, response_data, headers
  );

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

  res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    PRINTERR("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    goto fail;
  }

fail:
  free(header_data.response);
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);

  return res;
}
