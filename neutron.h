/*
Copyright (C) 2016-2022 Kotuku Aerospace Limited

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

If a file does not contain a copyright header, either because it is incomplete
or a binary file then the above copyright notice will apply.

Portions of this repository may have further copyright notices that may be
identified in the respective files.  In those cases the above copyright notice and
the GPL3 are subservient to that copyright notice.

Portions of this repository contain code fragments from the following
providers.


If any file has a copyright notice or portions of code have been used
and the original copyright notice is not yet transcribed to the repository
then the original copyright notice is to be respected.

If any material is included in the repository that is not open source
it must be removed as soon as possible after the code fragment is identified.

If you wish to use any of this code in a commercial application then
you must obtain a licence from the copyright holder.  Contact
support@kotuku.aero for information on the commercial licences.
*/
#ifndef __neutron_h__
#define __neutron_h__

#include <stdint.h>
#include <stdbool.h>

typedef int32_t result_t;
// Un-used
#define unused_id 0

// these are the default node settings

#define CANFLYID(id, num, type, descr) id = num,


enum canfly_id {

  #include "CanFlyID.def"

  };

#undef CANFLYID
// enumeration for a status field
typedef enum _e_board_status {
  bs_unknown = 0,
  bs_starting = 1,
  bs_running = 2,
  bs_inhibited = 3,
  bs_fault = 16,
  bs_bootrequest = 128,         // is 
  } e_board_status;

// A Status message is sent from each node.  Only one status message ID is used
// and the following bitfields are defined
typedef struct _status_msg_t {
  uint8_t msg_type;             // set to CANFLY_BINARY
  uint8_t node_id;              // is well known, or if kMAG can be the same id initially
  uint8_t board_status;         // see enum above
  uint8_t board_type;           // type of board
  uint32_t serial_number;       // board serial
} status_msg_t;


// these are used for the canflyID mapping
#define CANFLY_NODATA 0         // No data in the message
#define CANFLY_ERROR 1          // An error code is in the message
#define CANFLY_UINT8 2          // An unsigned integer
#define CANFLY_INT8 3           // An integer
#define CANFLY_UINT16 4         // A 16 bit integer
#define CANFLY_INT16 5          // A signed 16 bit integer
#define CANFLY_UINT32 6         // An unsigned 32 bit integer
#define CANFLY_INT32 7          // A signed 32 bit integer
#define CANFLY_BOOL_TRUE 8       // A true flag
#define CANFLY_BOOL_FALSE 9     // a false flag
#define CANFLY_FLOAT 10          // A floating point number
#define CANFLY_UTC 11           // an encoded UTC time, seconds since 2000-01-01

#define CANFLY_BINARY 0xFF      // the id is a binary type.

#define CANFLY_BOOL 0x100       // only used to send a flag, never sent

#define LENGTH_MASK 0xF000
#define ID_MASK 0x07FF
#define BINARY_MASK 0x08000

  /**
   * @struct canmsg_t
   * This is the message that is passed around the CanFly infrastructure
   * defined by Neutron
   * @param flags	Length, reply, ID
   * @param canas Encoded canas_msg_t
   */
  typedef struct _canmsg_t
    {
    // Bits 15:12 - Length
    // Bit 11 - binary data (used by a pipe)
    // bits 10-0 ID
    uint16_t flags;
    uint8_t data[8];
    } canmsg_t;

/**
 * @brief structure type for a time.
*/
typedef struct _tm_t {
  uint16_t year;
  uint16_t month;
  uint16_t day;
  uint16_t hour;
  uint16_t minute;
  uint16_t second;
  uint16_t milliseconds;
  } tm_t;

enum {
  s_orphaned = 3,
  s_dropped = 2,
  s_relocated = 1,
  s_ok = 0,
  s_false = -1,
  e_unexpected = -2,
  e_invalid_handle = -3,
  e_not_implemented = -4,
  e_bad_pointer = -5,
  e_bad_parameter = -6,
  e_more_data = -7,
  e_no_more_information = -8,
  e_path_not_found = -9,
  e_operation_pending = -10,
  e_operation_cancelled = -11,
  e_invalid_operation = -12,
  e_buffer_too_small = -13,
  e_generic_error = -14,
  e_timeout_error = -15,
  e_no_space = -16,
  e_not_enough_memory = -17,
  e_not_found = -18,
  e_bad_handle = -19,
  e_bad_ioctl = -20,
  e_ioctl_buffer_too_small = -21,
  e_not_initialized = -22,
  e_exists = -23,
  e_wrong_type = -24,
  e_parse_error = -25,
  e_incomplete_command = -26,
  e_bad_type = -27,
  e_not_supported = -28,
  e_out_of_range = -29,
  e_object_released = -30,
  e_unsupported_instruction = -31,
  e_overflow = -32,
  e_stream_closed = -33,
  e_corrupt = -34,
  e_not_directory = -35,
  e_not_file = -36,
  e_not_empty = -37,
  e_file_too_large = -38,
  e_no_attributes = -39,
  };

static inline bool failed(result_t r)
  {
  return r < 0;
  }

static inline bool succeeded(result_t r)
  {
  return r >= 0;
  }
/**
 * @brief encapsulated variant
*/
typedef enum _variant_type {
  v_none,
  v_bool,
  v_int8,
  v_uint8,
  v_int16,
  v_uint16,
  v_int32,
  v_uint32,
  v_float,
  v_utc
  } variant_type;

typedef  struct _variant_t {
  variant_type vt;
  union {
    bool boolean;
    int8_t int8;
    uint8_t uint8;
    int16_t int16;
    uint16_t uint16;
    int32_t int32;
    uint32_t uint32;
    float flt;
    tm_t utc;
    } value;
  } variant_t;

extern const variant_t *create_variant_nodata(variant_t *v);
extern const variant_t *create_variant_bool(bool value, variant_t *v);
extern const variant_t *create_variant_int8(int8_t value, variant_t *v);
extern const variant_t *create_variant_uint8(uint8_t value, variant_t *v);
extern const variant_t *create_variant_int16(int16_t value, variant_t *v);
extern const variant_t *create_variant_uint16(uint16_t value, variant_t *v);
extern const variant_t *create_variant_int32(int32_t value, variant_t *v);
extern const variant_t *create_variant_uint32(uint32_t value, variant_t *v);
extern const variant_t *create_variant_float(float value, variant_t *v);
extern const variant_t *create_variant_utc(const tm_t *value, variant_t *v);
extern result_t msg_to_variant(const canmsg_t *msg, variant_t *v);
extern result_t variant_to_msg(const variant_t *v, uint16_t id, uint16_t type, canmsg_t *msg);
extern result_t coerce_to_bool(const variant_t *src, bool *value);
extern result_t coerce_to_int8(const variant_t *src, int8_t *value);
extern result_t coerce_to_uint8(const variant_t *src, uint8_t *value);
extern result_t coerce_to_int16(const variant_t *src, int16_t *value);
extern result_t coerce_to_uint16(const variant_t *src, uint16_t *value);
extern result_t coerce_to_int32(const variant_t *src, int32_t *value);
extern result_t coerce_to_uint32(const variant_t *src, uint32_t *value);
extern result_t coerce_to_float(const variant_t *src, float *value);
extern result_t coerce_to_utc(const variant_t *src, tm_t *value);
extern result_t coerce_variant(const variant_t *src, variant_t *dst, variant_type to_type);
extern const variant_t *copy_variant(const variant_t *src, variant_t *dst);
extern int compare_variant(const variant_t *v1, const variant_t *v2);

/**
 * @brief Set the ID of a CANbus message
 * @param msg Message
 * @param id ID to assign
 * @remark This method should only be called for internal
 * messages (canID >= 1400 && canID < 1520) as the
 * published ID's all have very specific uses.
 * Use the create_can_msg_<datatype> functions to
 * create defined can messages
*/
static inline void set_can_id(canmsg_t *msg, uint16_t id)
  {
  msg->flags &= ~ID_MASK;
  msg->flags |= (id & ID_MASK);
  }
/**
 * @brief Get the length of a CANbus message
 * @param msg message
 * @return length
 * @remark The length of data types (can_id < 1520) is one more
 * than the length of the actual type.  The canfly protocol puts
 * the type of the data as the first byte of the message, followed
 * by a big endian encoded data field.
*/
static inline uint8_t get_can_len(const canmsg_t *msg)
  {
  return (uint8_t)((msg->flags & LENGTH_MASK) >> 12);
  }
/**
 * @brief Return the CANbus ID of the message
 * @param msg Message
 * @return ID of the message
*/
static inline uint16_t get_can_id(const canmsg_t *msg)
  {
  return msg->flags & ID_MASK;
  }
/**
 * @brief Return true if the can message is a binary format
 * @param msg Message
 * @return true if the message is a binary message
 * @remark This is returned true for all can ID's greater
 * that 1520 which is used by the pipe messages.
*/
static inline bool get_can_is_binary(const canmsg_t *msg)
  {
  return msg->flags * BINARY_MASK;
  }
/**
 * @brief Set a can message as binary
 * @param msg Message
 * @param is_it true if a binary message
*/
static inline void set_can_is_binary(canmsg_t *msg, bool is_it)
  {
  msg->flags &= ~BINARY_MASK;
  if (is_it)
    msg->flags |= BINARY_MASK;
  }
/**
 * @brief Return the encoded type of a message
 * @param msg Message
 * @return One of the CANFLY_ data types
*/
static inline uint16_t get_can_type(const canmsg_t *msg)
  {
  if ((msg->flags & BINARY_MASK) != 0)
    return CANFLY_BINARY;
  // non binary messages store the data type as the
  // first byte of the message
  switch(msg->data[0])
    {
    case CANFLY_BOOL_FALSE :
    case CANFLY_BOOL_TRUE :
      return CANFLY_BOOL;
    default:
      return msg->data[0];
    }
  }

/**
 * @brief create a status message
 * @param msg   message to create
 * @param node_id   Id of the node
 * @param node_type Type of node
 * @param status    Running status
 * @return s_ok if created ok
 */
extern result_t create_can_msg_status(canmsg_t *msg,
                               uint8_t node_id,
                               uint8_t node_type,
                               e_board_status status);
/**
 * @brief Decode an id_status message
 * @param msg       message
 * @param node_id   sender node id (0..14)
 * @param node_type Type of node (id_ahrs_id.. etc)
 * @param serial_number  Serial number of the board
 * @param status    Status of the board
 * @return 
 */
extern result_t get_param_status(const canmsg_t *msg,
                          uint8_t *node_id,
                          uint8_t *node_type,
                          uint32_t *serial_number,
                          e_board_status *status);

static inline bool is_status_msg(const canmsg_t *msg)
  {
  return get_can_id(msg) >= id_status_node_0 && get_can_id(msg) <= id_status_node_15;
  }

/**
 * @brief Create a can message sending an boolean value
 * @param msg           Message to construct
 * @param message_id    11 bit CanFly ID
 * @param data          Data to send
 *@return s_ok if created ok
 */
extern result_t create_can_msg_bool(canmsg_t *msg, uint16_t message_id, bool value);

/**
 * @fn const canmsg_t *create_can_msg_int8(canmsg_t *msg, uint16_t message_id, int8_t value)
 * @brief Create a can message sending an 8bit integer
 * @param msg           Message to construct
 * @param message_id    11 bit CanFly ID
 * @param data          Data to send
 *@return s_ok if created ok
 */
extern result_t create_can_msg_int8(canmsg_t *msg, uint16_t message_id, int8_t value);
/**
 * @fn const canmsg_t *create_can_msg_uint8(canmsg_t *msg, uint16_t message_id, uint8_t value)
 * @brief Create a can message sending an 8bit unsigned integer
 * @param msg           Message to construct
 * @param message_id    11 bit CanFly ID
 * @param value          Data to send
 *@return s_ok if created ok
 */
 extern result_t create_can_msg_uint8(canmsg_t *msg, uint16_t message_id, uint8_t value);
/**
 * @fn const canmsg_t *create_can_msg_int16(canmsg_t *msg, uint16_t message_id, int16_t data)
 * Create a can message sending a 16bit integer
 * @param msg           Message to construct
 * @param message_id    11 bit CanFly ID
 * @param data          Data to send
 *@return s_ok if created ok
 */
 extern result_t create_can_msg_int16(canmsg_t *msg, uint16_t message_id, int16_t data);
/**
 * @fn const canmsg_t *create_can_msg_uint16(canmsg_t *msg, uint16_t message_id, uint16_t data)
 * Create a can message sending a 16bit unsigned integer
 * @param msg           Message to construct
 * @param message_id    11 bit CanFly ID
 * @param data          Data to send
 *@return s_ok if created ok
 */
 extern result_t create_can_msg_uint16(canmsg_t *msg, uint16_t message_id, uint16_t data);
/**
 * @fn create_can_msg_int32(canmsg_t *msg, uint16_t message_id, int32_t data)
 * Create a can message sending a 32bit integer
 * @param msg           Message to construct
 * @param message_id    11 bit CanFly ID
 * @param data          Data to send
 *@return s_ok if created ok
 */
 extern result_t create_can_msg_int32(canmsg_t *msg, uint16_t message_id, int32_t data);
/**
 * @fn create_can_msg_uint32(canmsg_t *msg, uint16_t message_id, uint32_t data)
 * Create a can message sending a 32bit unsigned integer
 * @param msg           Message to construct
 * @param message_id    11 bit CanFly ID
 * @param data          Data to send
 *@return s_ok if created ok
 */
 extern result_t create_can_msg_uint32(canmsg_t *msg, uint16_t message_id, uint32_t data);
/**
 * @fn create_can_msg_float(canmsg_t *msg, uint16_t message_id, float data)
 * Create a can message sending a floating point value
 * @param msg           Message to construct
 * @param message_id    11 bit CanFly ID
 * @param data          Data to send
 *@return s_ok if created ok
 */
 extern result_t create_can_msg_float(canmsg_t *msg, uint16_t message_id, float data);
/**
 * @fn create_can_msg_utc(canmsg_t *msg, uint16_t message_id, const tm_t *value)
 * @brief Create a can message sending a utc time
 * @param msg           Message to construct
 * @param message_id    11 bit CanFly ID
 * @param data          Data to send
 *@return s_ok if created ok
 */
 extern result_t create_can_msg_utc(canmsg_t *msg, uint16_t message_id, const tm_t *value);
 /**
  * @brief create an empty message
  * @param msg Message to constuct
  * @param message_id 11 bit CanFly ID
  * @return s_ok if created ok
 */
 extern result_t create_can_msg_nodata(canmsg_t *msg, uint16_t message_id);
 /**
  * @brief Create a can message with an error code
 * @param msg           Message to construct
 * @param message_id    11 bit CanFly ID
  * @param error        Error to send
  * @return  s_ok if created ok
 */
 extern result_t create_can_msg_error(canmsg_t *msg, uint16_t message_id, uint32_t error);
/**
 * @fn get_param_int8(const canmsg_t *msg, int8_t *value)
 * @param msg         Message to extract parameter from
 * @param value       value of the message
 * @return s_ok if the message can be coerced to an int8, and the data is returned
 */
 extern result_t get_param_bool(const canmsg_t *msg, bool *value);
/**
 * @fn get_param_int8(const canmsg_t *msg, int8_t *value)
 * @param msg         Message to extract parameter from
 * @param value       value of the message
 * @return s_ok if the message can be coerced to an int8, and the data is returned
 */
 extern result_t get_param_int8(const canmsg_t *msg, int8_t *value);
/**
* @fn get_param_uint8(const canmsg_t *msg, uint8_t *value)
* @param msg         Message to extract parameter from
* @param value       extracted value
* @return s_ok if the message can be coerced to an uint8, and the data is returned
*/
extern result_t get_param_uint8(const canmsg_t *msg, uint8_t *value);
/**
 * @fn get_param_int16(const canmsg_t *msg, int16_t *value)
 * @param msg         Message to extract parameter from
 * @param value       extracted value
 * @return s_ok if the message can be coerced to an int16, and the data is returned
 */
 extern result_t get_param_int16(const canmsg_t *msg, int16_t *value);
/**
 * @fn get_param_uint16(const canmsg_t *msg, uint16_t *value)
 * @param msg         Message to extract parameter from
 * @param index       Index of the value (0..1)
 * @param value       extracted value
 * @return s_ok if the message can be coerced to an int, and the data is returned
 */
 extern result_t get_param_uint16(const canmsg_t *msg, uint16_t *value);
/**
 * @fn get_param_int32(const canmsg_t *msg, int32_t *value)
 * @param msg         Message to extract parameter from
 * @param value       extracted value
 * @return s_ok if the message can be coerced to an int32, and the data is returned
 */
 extern result_t get_param_int32(const canmsg_t *msg, int32_t *value);
/**
 * @fn get_param_uint32(const canmsg_t *msg, uint32_t *value)
 * @param msg         Message to extract parameter from
 * @param value       extracted value
 * @return s_ok if the message can be coerced to an uint32, and the data is returned
 */
 extern result_t get_param_uint32(const canmsg_t *msg, uint32_t *value);
/**
 * @brief Unpack the UTC message into a time structure
 * @param msg   message to unpack
 * @param value time values, note milliseconds is set to 0
 * @return s_ok if the message was a valid utc message
*/
 extern result_t get_param_utc(const canmsg_t *msg, tm_t *value);
/**
 * @fn get_param_float(const canmsg_t *msg, float *value)
 * @param msg         Message to extract parameter from
 * @param value       extracted value
 * @return s_ok if the message can be coerced to a float, and the data is returned
 */
 extern result_t get_param_float(const canmsg_t *msg, float *value);


#endif
