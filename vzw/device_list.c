#include "../curl/helpers.h"
#include "api_fields.h"

#define ACCOUNT_NAME_FIELD_SIZE(aname) sizeof(ACCOUNT_NAME_FIELD) + strlen(aname) + 3

int vzw_global_device_list(
    const char *account_name, char *auth_token, char *session_token, VZWResponseData *response_data
) {
  char *ptr;
  CURL *curl = curl_easy_init();
  CURLcode res;
  struct curl_slist *headers = NULL;
  VZWResponseData header_data = {NULL, 0};

  char post_field[ACCOUNT_NAME_FIELD_SIZE(account_name)];
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
  ptr = stpcpy(&post_field[1], ACCOUNT_NAME_FIELD);
  ptr = stpcpy(ptr, account_name);
  (void)stpcpy(ptr, "\"}");

  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_field);
  curl_easy_setopt(curl, CURLOPT_URL, VZW_M2M_REST_API_V1 "/devices/actions/list");
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
