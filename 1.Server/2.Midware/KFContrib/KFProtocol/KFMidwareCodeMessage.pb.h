// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: KFMidwareCodeMessage.proto

#ifndef PROTOBUF_INCLUDED_KFMidwareCodeMessage_2eproto
#define PROTOBUF_INCLUDED_KFMidwareCodeMessage_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_KFMidwareCodeMessage_2eproto LIBPROTOC_EXPORT

namespace protobuf_KFMidwareCodeMessage_2eproto {
// Internal implementation detail -- do not use these members.
struct LIBPROTOC_EXPORT TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void LIBPROTOC_EXPORT AddDescriptors();
}  // namespace protobuf_KFMidwareCodeMessage_2eproto
namespace KFMsg {
}  // namespace KFMsg
namespace KFMsg {

enum MidwareCodeEnum {
  Error = 0,
  Ok = 1,
  HttpDataError = 10000,
  AuthDatabaseBusy = 10001,
  AuthServerBusy = 10002,
  ZoneServerBusy = 10003,
  ZoneDatabaseBusy = 10004,
  AccountIsEmpty = 11001,
  ActivationAccount = 11002,
  InvalidActivationCode = 11003,
  ActivationCodeError = 11004,
  LoginTokenError = 11005,
  BanForbidLogin = 11006,
  ChannelNotSupport = 11007,
  ChannelNotOpen = 11008,
  ChannelError = 11009,
  WeiXinError = 11010,
  WeiXinUserError = 11011,
  WeiXinTokenError = 11012,
  WeiXinCodeError = 11013,
  WeiXinTokenTimeout = 11014,
  SteamError = 11015,
  SteamDataError = 11016,
  SteamAuthError = 11017,
  MidwareCodeEnum_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  MidwareCodeEnum_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
LIBPROTOC_EXPORT bool MidwareCodeEnum_IsValid(int value);
const MidwareCodeEnum MidwareCodeEnum_MIN = Error;
const MidwareCodeEnum MidwareCodeEnum_MAX = SteamAuthError;
const int MidwareCodeEnum_ARRAYSIZE = MidwareCodeEnum_MAX + 1;

LIBPROTOC_EXPORT const ::google::protobuf::EnumDescriptor* MidwareCodeEnum_descriptor();
inline const ::std::string& MidwareCodeEnum_Name(MidwareCodeEnum value) {
  return ::google::protobuf::internal::NameOfEnum(
    MidwareCodeEnum_descriptor(), value);
}
inline bool MidwareCodeEnum_Parse(
    const ::std::string& name, MidwareCodeEnum* value) {
  return ::google::protobuf::internal::ParseNamedEnum<MidwareCodeEnum>(
    MidwareCodeEnum_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace KFMsg

namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::KFMsg::MidwareCodeEnum> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::KFMsg::MidwareCodeEnum>() {
  return ::KFMsg::MidwareCodeEnum_descriptor();
}

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_KFMidwareCodeMessage_2eproto