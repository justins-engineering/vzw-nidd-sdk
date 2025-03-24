#include <base64.h>
#include <curl/curl.h>
#include <curl/header.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../curl/helpers.h"
#include "api_fields.h"

#define DEVICE_ID_FIELD_START "\"deviceIds\":[{\"id\":\""
#define DEVICE_ID_FIELD_END "\",\"kind\":\"MDN\"}],"

#define ACCOUNT_NAME_FIELD "\"accountName\":\""
#define MDT_FIELD "\"maximumDeliveryTime\":\""
#define MESSAGE_FIELD "\"message\":\""

#define MDN_SIZE 10
#define MAX_MDT_SIZE 7
#define MAX_MESSAGE_SIZE 10864

#define SEND_NIDD_BODY_MAX_SIZE(aname)                                                       \
  sizeof(DEVICE_ID_FIELD_START) + sizeof(DEVICE_ID_FIELD_END) + sizeof(ACCOUNT_NAME_FIELD) + \
      strlen(aname) + sizeof(MDT_FIELD) + sizeof(MESSAGE_FIELD) + MDN_SIZE + MAX_MDT_SIZE +  \
      MAX_MESSAGE_SIZE + 3

int vzw_send_nidd_data(
    const char *account_name, char *auth_token, char *session_token, char *mdn, char *mdt,
    char *message, CharBuff *response_data
) {
  char *ptr;
  CURL *curl = curl_easy_init();
  CURLcode res;
  struct curl_slist *headers = NULL;
  CharBuff header_data = {NULL, 0};

  if (BASE64LEN(strlen(message)) > MAX_MESSAGE_SIZE) {
    PRINTERR("NIDD message length too large");
    return 1;
  }

  char post_field[SEND_NIDD_BODY_MAX_SIZE(account_name)];
  char access_token_field[ACCESS_TOKEN_FIELD_SIZE(auth_token)];
  char session_token_field[SESSION_TOKEN_FIELD_SIZE(session_token)];

  ptr = stpcpy(access_token_field, ACCESS_TOKEN_FIELD);
  (void)stpcpy(ptr, auth_token);

  ptr = stpcpy(session_token_field, SESSION_TOKEN_FIELD);
  (void)stpcpy(ptr, session_token);

  headers = curl_slist_append(headers, CONTENT_TYPE_JSON);
  headers = curl_slist_append(headers, ACCEPT_JSON);
  headers = curl_slist_append(headers, access_token_field);
  headers = curl_slist_append(headers, session_token_field);

  post_field[0] = '{';
  ptr = stpcpy(&post_field[1], DEVICE_ID_FIELD_START);
  ptr = stpcpy(ptr, mdn);
  ptr = stpcpy(ptr, DEVICE_ID_FIELD_END);
  ptr = stpcpy(ptr, ACCOUNT_NAME_FIELD);
  ptr = stpcpy(ptr, account_name);
  ptr = stpcpy(ptr, "\",");
  ptr = stpcpy(ptr, MDT_FIELD);
  ptr = stpcpy(ptr, mdt);
  ptr = stpcpy(ptr, "\",");
  ptr = stpcpy(ptr, MESSAGE_FIELD);
  ptr += base64(message, strlen(message), ptr);
  (void)stpcpy(ptr, "\"}");

  // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_field);
  curl_easy_setopt(curl, CURLOPT_URL, VZW_M2M_REST_API "/devices/nidd/message");
  curl_easy_default_opts(curl, &header_data, response_data, headers);

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
