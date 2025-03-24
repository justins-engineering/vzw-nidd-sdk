/** @file api_fields.h
 *  @brief Shared definitions and macros for the vzw thingspace APIs
 */

#ifndef API_FIELDS_H
#define API_FIELDS_H

#include <curl/curl.h>
#include <curl/header.h>
#include <definitions.h>
#include <stdlib.h>
#include <string.h>

#define CONTENT_TYPE_JSON "Content-Type:application/json"
#define ACCEPT_JSON "Accept:application/json"

#define VZW_M2M_REST_API_V1 "https://thingspace.verizon.com/api/m2m/v1"
#define VZW_M2M_REST_API_V2 "https://thingspace.verizon.com/api/m2m/v2"

#define OAUTH2_TOKEN_FIELD "Authorization:Basic "

/** Base64 encoding is deterministic, so we know the length ahead of time */
#define OAUTH2_TOKEN_FIELD_SIZE sizeof(OAUTH2_TOKEN_FIELD) + BASE64LEN(strlen(auth_keys))

#define ACCESS_TOKEN_FIELD "Authorization:Bearer "
#define ACCESS_TOKEN_FIELD_SIZE(t) sizeof(ACCESS_TOKEN_FIELD) + strlen(t)

#define SESSION_TOKEN_FIELD "VZ-M2M-Token:"
#define SESSION_TOKEN_FIELD_SIZE(t) sizeof(SESSION_TOKEN_FIELD) + strlen(t)

#define VZW_CALLBACKS_API VZW_M2M_REST_API_V1 "/callbacks/"
#define VZW_CALLBACKS_API_LEN(aname) sizeof(VZW_M2M_REST_API_V1) + strlen(aname)

#define ACCOUNT_NAME_FIELD "\"accountName\":\""

#endif  // API_FIELDS_H
