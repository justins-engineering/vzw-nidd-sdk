/** @file vzw_nidd.h
 *  @brief Macro and function defines for the vzw thingspace credentials.
 */

#ifndef VZW_NIDD_H
#define VZW_NIDD_H

#include <stdio.h>

#define PRINTDBG(f_, ...) fprintf(stderr, "%s" f_ "%s", "\033[38;5;87m", ##__VA_ARGS__, "\033[0m\n")

#define PRINTERR(f_, ...) fprintf(stderr, "%s" f_ "%s", "\033[38;5;9m", ##__VA_ARGS__, "\033[0m\n")

#define PRINTALRT(f_, ...) fprintf(stderr, "%s" f_ "%s", "\033[38;5;226m", ##__VA_ARGS__, "\033[0m\n")

#define PRINTSUCCES(f_, ...) fprintf(stderr, "%s" f_ "%s", "\033[38;5;82m", ##__VA_ARGS__, "\033[0m\n")

typedef struct CharBuff {
  char *response;
  size_t size;
} CharBuff;

/** @fn int vzw_get_auth_token(const char *username, const char *password, const
*   char *auth_keys, char *auth_token)
*   @brief Retrieves a Thingspace auth token through a POST request.
*/
int vzw_get_auth_token(const char *auth_keys, char *auth_token);

/** @fn int vzw_get_session_token(const char *username, const char *password, const
*   char *auth_token, char *session_token)
*   @brief Retrieves a Thingspace session token through a POST request.
*/
int vzw_get_session_token(
    const char *username, const char *password, const char *auth_token, char *session_token
);

int vzw_get_registered_callback_listeners(
  const char *account_name, char *auth_token, char *session_token, CharBuff *response_data
);

int vzw_set_registered_callback_listeners(
  const char *account_name, char *auth_token, char *session_token, CharBuff *response_data,
  char *service_name, char *url
);

int vzw_delete_registered_callback_listeners(
  const char *account_name, char *auth_token, char *session_token, CharBuff *response_data,
  char *service_name
);

int vzw_send_nidd_data(
  const char *account_name, char *auth_token, char *session_token, char *mdn, char *mdt,
  char *message, CharBuff *response_data
);

#endif // VZW_NIDD_H
