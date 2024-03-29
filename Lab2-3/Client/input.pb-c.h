/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: input.proto */

#ifndef PROTOBUF_C_input_2eproto__INCLUDED
#define PROTOBUF_C_input_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1005000 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct UintArray UintArray;
typedef struct DoubleArray DoubleArray;
typedef struct StringArray StringArray;
typedef struct Node Node;


/* --- enums --- */


/* --- messages --- */

struct  UintArray
{
  ProtobufCMessage base;
  uint64_t value;
  UintArray *next;
};
#define UINT_ARRAY__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&uint_array__descriptor) \
, 0, NULL }


struct  DoubleArray
{
  ProtobufCMessage base;
  double value;
  DoubleArray *next;
};
#define DOUBLE_ARRAY__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&double_array__descriptor) \
, 0, NULL }


struct  StringArray
{
  ProtobufCMessage base;
  char *value;
  StringArray *next;
};
#define STRING_ARRAY__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&string_array__descriptor) \
, (char *)protobuf_c_empty_string, NULL }


struct  Node
{
  ProtobufCMessage base;
  uint64_t id;
  uint64_t d;
  UintArray *nodes;
  Node *next_node;
  uint64_t n_ints;
  UintArray *ints_array_ptr;
  uint64_t n_doubles;
  DoubleArray *doubles_array_ptr;
  uint64_t n_strings;
  StringArray *strings_array_ptr;
  char *msg;
};
#define NODE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&node__descriptor) \
, 0, 0, NULL, NULL, 0, NULL, 0, NULL, 0, NULL, (char *)protobuf_c_empty_string }


/* UintArray methods */
void   uint_array__init
                     (UintArray         *message);
size_t uint_array__get_packed_size
                     (const UintArray   *message);
size_t uint_array__pack
                     (const UintArray   *message,
                      uint8_t             *out);
size_t uint_array__pack_to_buffer
                     (const UintArray   *message,
                      ProtobufCBuffer     *buffer);
UintArray *
       uint_array__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   uint_array__free_unpacked
                     (UintArray *message,
                      ProtobufCAllocator *allocator);
/* DoubleArray methods */
void   double_array__init
                     (DoubleArray         *message);
size_t double_array__get_packed_size
                     (const DoubleArray   *message);
size_t double_array__pack
                     (const DoubleArray   *message,
                      uint8_t             *out);
size_t double_array__pack_to_buffer
                     (const DoubleArray   *message,
                      ProtobufCBuffer     *buffer);
DoubleArray *
       double_array__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   double_array__free_unpacked
                     (DoubleArray *message,
                      ProtobufCAllocator *allocator);
/* StringArray methods */
void   string_array__init
                     (StringArray         *message);
size_t string_array__get_packed_size
                     (const StringArray   *message);
size_t string_array__pack
                     (const StringArray   *message,
                      uint8_t             *out);
size_t string_array__pack_to_buffer
                     (const StringArray   *message,
                      ProtobufCBuffer     *buffer);
StringArray *
       string_array__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   string_array__free_unpacked
                     (StringArray *message,
                      ProtobufCAllocator *allocator);
/* Node methods */
void   node__init
                     (Node         *message);
size_t node__get_packed_size
                     (const Node   *message);
size_t node__pack
                     (const Node   *message,
                      uint8_t             *out);
size_t node__pack_to_buffer
                     (const Node   *message,
                      ProtobufCBuffer     *buffer);
Node *
       node__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   node__free_unpacked
                     (Node *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*UintArray_Closure)
                 (const UintArray *message,
                  void *closure_data);
typedef void (*DoubleArray_Closure)
                 (const DoubleArray *message,
                  void *closure_data);
typedef void (*StringArray_Closure)
                 (const StringArray *message,
                  void *closure_data);
typedef void (*Node_Closure)
                 (const Node *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor uint_array__descriptor;
extern const ProtobufCMessageDescriptor double_array__descriptor;
extern const ProtobufCMessageDescriptor string_array__descriptor;
extern const ProtobufCMessageDescriptor node__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_input_2eproto__INCLUDED */
