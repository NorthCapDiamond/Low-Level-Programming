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


typedef struct String String;
typedef struct Attr Attr;
typedef struct Set Set;
typedef struct Link Link;
typedef struct AttrType AttrType;
typedef struct Value Value;
typedef struct Ast Ast;


/* --- enums --- */

typedef enum _AstNodeType {
  AST_NODE_TYPE__FILENAME_N = 0,
  AST_NODE_TYPE__INSERT_N = 1,
  AST_NODE_TYPE__SELECT_N = 2,
  AST_NODE_TYPE__UPDATE_N = 3,
  AST_NODE_TYPE__DELETE_N = 4,
  AST_NODE_TYPE__INDEX_N = 5,
  AST_NODE_TYPE__NODE_N = 6,
  AST_NODE_TYPE__LINK_N = 7,
  AST_NODE_TYPE__CONDITIONAL_N = 8,
  AST_NODE_TYPE__FILTER_N = 9,
  AST_NODE_TYPE__SET_N = 10,
  AST_NODE_TYPE__VAL_N = 11,
  AST_NODE_TYPE__NAME_N = 12,
  AST_NODE_TYPE__LIST_N = 13,
  AST_NODE_TYPE__ATTR_DESC_N = 14
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(AST_NODE_TYPE)
} AstNodeType;
typedef enum _InsertTarget {
  INSERT_TARGET__I_NODE = 0,
  INSERT_TARGET__I_LINK = 1,
  INSERT_TARGET__I_INDEX = 2
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(INSERT_TARGET)
} InsertTarget;
typedef enum _ValType {
  VAL_TYPE__INT = 0,
  VAL_TYPE__DOUBLE = 1,
  VAL_TYPE__STRING = 2,
  VAL_TYPE__BOOL = 3
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(VAL_TYPE)
} ValType;
typedef enum _LogicalOp {
  LOGICAL_OP__AND = 0,
  LOGICAL_OP__OR = 1,
  LOGICAL_OP__NOT = 2
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(LOGICAL_OP)
} LogicalOp;
typedef enum _Cmp {
  CMP__GT = 0,
  CMP__LT = 1,
  CMP__EQ = 2,
  CMP__GT_EQ = 3,
  CMP__LT_EQ = 4,
  CMP__SUBSTR = 5
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(CMP)
} Cmp;
typedef enum _ValueType {
  VALUE_TYPE__NONE = 0,
  VALUE_TYPE__NAME = 1,
  VALUE_TYPE__INSERT_TARGET = 2,
  VALUE_TYPE__SET = 3,
  VALUE_TYPE__ATTR = 4,
  VALUE_TYPE__LOGICAL_OP = 5,
  VALUE_TYPE__CMP = 6,
  VALUE_TYPE__LINK = 7,
  VALUE_TYPE__CNT = 8,
  VALUE_TYPE__ATTR_DESC = 9
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(VALUE_TYPE)
} ValueType;

/* --- messages --- */

struct  String
{
  ProtobufCMessage base;
  uint32_t size;
  char *str;
};
#define STRING__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&string__descriptor) \
, 0, (char *)protobuf_c_empty_string }


struct  Attr
{
  ProtobufCMessage base;
  ValType type;
  int32_t i;
  double d;
  String *str;
  protobuf_c_boolean b;
};
#define ATTR__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&attr__descriptor) \
, VAL_TYPE__INT, 0, 0, NULL, 0 }


struct  Set
{
  ProtobufCMessage base;
  uint32_t node_id;
  char *attr_name;
  Attr *new_value;
};
#define SET__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&set__descriptor) \
, 0, (char *)protobuf_c_empty_string, NULL }


struct  Link
{
  ProtobufCMessage base;
  uint32_t link_id_one;
  uint32_t link_id_two;
};
#define LINK__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&link__descriptor) \
, 0, 0 }


struct  AttrType
{
  ProtobufCMessage base;
  char *name;
  ValType val;
};
#define ATTR__TYPE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&attr__type__descriptor) \
, (char *)protobuf_c_empty_string, VAL_TYPE__INT }


struct  Value
{
  ProtobufCMessage base;
  char *name;
  InsertTarget target;
  Set *set;
  Attr *attr;
  LogicalOp l_op;
  Cmp cmp;
  Link *link;
  uint32_t cnt;
  AttrType *attr_type;
};
#define VALUE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&value__descriptor) \
, (char *)protobuf_c_empty_string, INSERT_TARGET__I_NODE, NULL, NULL, LOGICAL_OP__AND, CMP__GT, NULL, 0, NULL }


struct  Ast
{
  ProtobufCMessage base;
  Ast *left;
  Ast *right;
  AstNodeType type;
  ValueType v_type;
  Value *val;
};
#define AST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&ast__descriptor) \
, NULL, NULL, AST_NODE_TYPE__FILENAME_N, VALUE_TYPE__NONE, NULL }


/* String methods */
void   string__init
                     (String         *message);
size_t string__get_packed_size
                     (const String   *message);
size_t string__pack
                     (const String   *message,
                      uint8_t             *out);
size_t string__pack_to_buffer
                     (const String   *message,
                      ProtobufCBuffer     *buffer);
String *
       string__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   string__free_unpacked
                     (String *message,
                      ProtobufCAllocator *allocator);
/* Attr methods */
void   attr__init
                     (Attr         *message);
size_t attr__get_packed_size
                     (const Attr   *message);
size_t attr__pack
                     (const Attr   *message,
                      uint8_t             *out);
size_t attr__pack_to_buffer
                     (const Attr   *message,
                      ProtobufCBuffer     *buffer);
Attr *
       attr__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   attr__free_unpacked
                     (Attr *message,
                      ProtobufCAllocator *allocator);
/* Set methods */
void   set__init
                     (Set         *message);
size_t set__get_packed_size
                     (const Set   *message);
size_t set__pack
                     (const Set   *message,
                      uint8_t             *out);
size_t set__pack_to_buffer
                     (const Set   *message,
                      ProtobufCBuffer     *buffer);
Set *
       set__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   set__free_unpacked
                     (Set *message,
                      ProtobufCAllocator *allocator);
/* Link methods */
void   link__init
                     (Link         *message);
size_t link__get_packed_size
                     (const Link   *message);
size_t link__pack
                     (const Link   *message,
                      uint8_t             *out);
size_t link__pack_to_buffer
                     (const Link   *message,
                      ProtobufCBuffer     *buffer);
Link *
       link__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   link__free_unpacked
                     (Link *message,
                      ProtobufCAllocator *allocator);
/* AttrType methods */
void   attr__type__init
                     (AttrType         *message);
size_t attr__type__get_packed_size
                     (const AttrType   *message);
size_t attr__type__pack
                     (const AttrType   *message,
                      uint8_t             *out);
size_t attr__type__pack_to_buffer
                     (const AttrType   *message,
                      ProtobufCBuffer     *buffer);
AttrType *
       attr__type__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   attr__type__free_unpacked
                     (AttrType *message,
                      ProtobufCAllocator *allocator);
/* Value methods */
void   value__init
                     (Value         *message);
size_t value__get_packed_size
                     (const Value   *message);
size_t value__pack
                     (const Value   *message,
                      uint8_t             *out);
size_t value__pack_to_buffer
                     (const Value   *message,
                      ProtobufCBuffer     *buffer);
Value *
       value__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   value__free_unpacked
                     (Value *message,
                      ProtobufCAllocator *allocator);
/* Ast methods */
void   ast__init
                     (Ast         *message);
size_t ast__get_packed_size
                     (const Ast   *message);
size_t ast__pack
                     (const Ast   *message,
                      uint8_t             *out);
size_t ast__pack_to_buffer
                     (const Ast   *message,
                      ProtobufCBuffer     *buffer);
Ast *
       ast__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   ast__free_unpacked
                     (Ast *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*String_Closure)
                 (const String *message,
                  void *closure_data);
typedef void (*Attr_Closure)
                 (const Attr *message,
                  void *closure_data);
typedef void (*Set_Closure)
                 (const Set *message,
                  void *closure_data);
typedef void (*Link_Closure)
                 (const Link *message,
                  void *closure_data);
typedef void (*AttrType_Closure)
                 (const AttrType *message,
                  void *closure_data);
typedef void (*Value_Closure)
                 (const Value *message,
                  void *closure_data);
typedef void (*Ast_Closure)
                 (const Ast *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCEnumDescriptor    ast_node_type__descriptor;
extern const ProtobufCEnumDescriptor    insert_target__descriptor;
extern const ProtobufCEnumDescriptor    val_type__descriptor;
extern const ProtobufCEnumDescriptor    logical_op__descriptor;
extern const ProtobufCEnumDescriptor    cmp__descriptor;
extern const ProtobufCEnumDescriptor    value_type__descriptor;
extern const ProtobufCMessageDescriptor string__descriptor;
extern const ProtobufCMessageDescriptor attr__descriptor;
extern const ProtobufCMessageDescriptor set__descriptor;
extern const ProtobufCMessageDescriptor link__descriptor;
extern const ProtobufCMessageDescriptor attr__type__descriptor;
extern const ProtobufCMessageDescriptor value__descriptor;
extern const ProtobufCMessageDescriptor ast__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_input_2eproto__INCLUDED */
