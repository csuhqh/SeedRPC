// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/protobuf/unittest_import_public.proto

#include "google/protobuf/unittest_import_public.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
namespace protobuf_unittest_import {
constexpr PublicImportMessage::PublicImportMessage(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : e_(0){}
struct PublicImportMessageDefaultTypeInternal {
  constexpr PublicImportMessageDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~PublicImportMessageDefaultTypeInternal() {}
  union {
    PublicImportMessage _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PublicImportMessageDefaultTypeInternal _PublicImportMessage_default_instance_;
}  // namespace protobuf_unittest_import
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_google_2fprotobuf_2funittest_5fimport_5fpublic_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_google_2fprotobuf_2funittest_5fimport_5fpublic_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_google_2fprotobuf_2funittest_5fimport_5fpublic_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_google_2fprotobuf_2funittest_5fimport_5fpublic_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::protobuf_unittest_import::PublicImportMessage, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::protobuf_unittest_import::PublicImportMessage, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::protobuf_unittest_import::PublicImportMessage, e_),
  0,
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 6, sizeof(::protobuf_unittest_import::PublicImportMessage)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::protobuf_unittest_import::_PublicImportMessage_default_instance_),
};

const char descriptor_table_protodef_google_2fprotobuf_2funittest_5fimport_5fpublic_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n,google/protobuf/unittest_import_public"
  ".proto\022\030protobuf_unittest_import\" \n\023Publ"
  "icImportMessage\022\t\n\001e\030\001 \001(\005B\032\n\030com.google"
  ".protobuf.test"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_google_2fprotobuf_2funittest_5fimport_5fpublic_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_google_2fprotobuf_2funittest_5fimport_5fpublic_2eproto = {
  false, false, 134, descriptor_table_protodef_google_2fprotobuf_2funittest_5fimport_5fpublic_2eproto, "google/protobuf/unittest_import_public.proto", 
  &descriptor_table_google_2fprotobuf_2funittest_5fimport_5fpublic_2eproto_once, nullptr, 0, 1,
  schemas, file_default_instances, TableStruct_google_2fprotobuf_2funittest_5fimport_5fpublic_2eproto::offsets,
  file_level_metadata_google_2fprotobuf_2funittest_5fimport_5fpublic_2eproto, file_level_enum_descriptors_google_2fprotobuf_2funittest_5fimport_5fpublic_2eproto, file_level_service_descriptors_google_2fprotobuf_2funittest_5fimport_5fpublic_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_google_2fprotobuf_2funittest_5fimport_5fpublic_2eproto_getter() {
  return &descriptor_table_google_2fprotobuf_2funittest_5fimport_5fpublic_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_google_2fprotobuf_2funittest_5fimport_5fpublic_2eproto(&descriptor_table_google_2fprotobuf_2funittest_5fimport_5fpublic_2eproto);
namespace protobuf_unittest_import {

// ===================================================================

class PublicImportMessage::_Internal {
 public:
  using HasBits = decltype(std::declval<PublicImportMessage>()._has_bits_);
  static void set_has_e(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
};

PublicImportMessage::PublicImportMessage(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:protobuf_unittest_import.PublicImportMessage)
}
PublicImportMessage::PublicImportMessage(const PublicImportMessage& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  e_ = from.e_;
  // @@protoc_insertion_point(copy_constructor:protobuf_unittest_import.PublicImportMessage)
}

void PublicImportMessage::SharedCtor() {
e_ = 0;
}

PublicImportMessage::~PublicImportMessage() {
  // @@protoc_insertion_point(destructor:protobuf_unittest_import.PublicImportMessage)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void PublicImportMessage::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void PublicImportMessage::ArenaDtor(void* object) {
  PublicImportMessage* _this = reinterpret_cast< PublicImportMessage* >(object);
  (void)_this;
}
void PublicImportMessage::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void PublicImportMessage::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void PublicImportMessage::Clear() {
// @@protoc_insertion_point(message_clear_start:protobuf_unittest_import.PublicImportMessage)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  e_ = 0;
  _has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* PublicImportMessage::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // optional int32 e = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          _Internal::set_has_e(&has_bits);
          e_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag == 0) || ((tag & 7) == 4)) {
          CHK_(ptr);
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  _has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* PublicImportMessage::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:protobuf_unittest_import.PublicImportMessage)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // optional int32 e = 1;
  if (cached_has_bits & 0x00000001u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(1, this->_internal_e(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:protobuf_unittest_import.PublicImportMessage)
  return target;
}

size_t PublicImportMessage::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:protobuf_unittest_import.PublicImportMessage)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // optional int32 e = 1;
  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000001u) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_e());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void PublicImportMessage::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:protobuf_unittest_import.PublicImportMessage)
  GOOGLE_DCHECK_NE(&from, this);
  const PublicImportMessage* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<PublicImportMessage>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:protobuf_unittest_import.PublicImportMessage)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:protobuf_unittest_import.PublicImportMessage)
    MergeFrom(*source);
  }
}

void PublicImportMessage::MergeFrom(const PublicImportMessage& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:protobuf_unittest_import.PublicImportMessage)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_has_e()) {
    _internal_set_e(from._internal_e());
  }
}

void PublicImportMessage::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:protobuf_unittest_import.PublicImportMessage)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void PublicImportMessage::CopyFrom(const PublicImportMessage& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:protobuf_unittest_import.PublicImportMessage)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool PublicImportMessage::IsInitialized() const {
  return true;
}

void PublicImportMessage::InternalSwap(PublicImportMessage* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  swap(e_, other->e_);
}

::PROTOBUF_NAMESPACE_ID::Metadata PublicImportMessage::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_google_2fprotobuf_2funittest_5fimport_5fpublic_2eproto_getter, &descriptor_table_google_2fprotobuf_2funittest_5fimport_5fpublic_2eproto_once,
      file_level_metadata_google_2fprotobuf_2funittest_5fimport_5fpublic_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace protobuf_unittest_import
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::protobuf_unittest_import::PublicImportMessage* Arena::CreateMaybeMessage< ::protobuf_unittest_import::PublicImportMessage >(Arena* arena) {
  return Arena::CreateMessageInternal< ::protobuf_unittest_import::PublicImportMessage >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
