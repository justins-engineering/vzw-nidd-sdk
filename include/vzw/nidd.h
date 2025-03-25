/** @file nidd.h
 *  @brief Macro and function defines for the vzw thingspace NIDD API.
 */

#ifndef NIDD_H
#define NIDD_H

int vzw_send_nidd_data(
    const char *account_name, char *auth_token, char *session_token, char *mdn, char *mdt,
    char *message, void *response_data
);

#endif  // NIDD_H
