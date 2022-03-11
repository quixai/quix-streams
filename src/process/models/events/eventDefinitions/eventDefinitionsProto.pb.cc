// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: eventDefinitionsProto.proto

#include "eventDefinitionsProto.pb.h"

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
namespace Quix {
namespace Process {
constexpr EventDefinitionProto::EventDefinitionProto(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : id_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string)
  , location_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string)
  , level_(0)
  , _oneof_case_{}{}
struct EventDefinitionProtoDefaultTypeInternal {
  constexpr EventDefinitionProtoDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~EventDefinitionProtoDefaultTypeInternal() {}
  union {
    EventDefinitionProto _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT EventDefinitionProtoDefaultTypeInternal _EventDefinitionProto_default_instance_;
constexpr EventDefinitionsProto::EventDefinitionsProto(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : events_(){}
struct EventDefinitionsProtoDefaultTypeInternal {
  constexpr EventDefinitionsProtoDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~EventDefinitionsProtoDefaultTypeInternal() {}
  union {
    EventDefinitionsProto _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT EventDefinitionsProtoDefaultTypeInternal _EventDefinitionsProto_default_instance_;
}  // namespace Process
}  // namespace Quix
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_eventDefinitionsProto_2eproto[2];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_eventDefinitionsProto_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_eventDefinitionsProto_2eproto = nullptr;

const uint32_t TableStruct_eventDefinitionsProto_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::Quix::Process::EventDefinitionProto, _internal_metadata_),
  ~0u,  // no _extensions_
  PROTOBUF_FIELD_OFFSET(::Quix::Process::EventDefinitionProto, _oneof_case_[0]),
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::Quix::Process::EventDefinitionProto, id_),
  ::PROTOBUF_NAMESPACE_ID::internal::kInvalidFieldOffsetTag,
  ::PROTOBUF_NAMESPACE_ID::internal::kInvalidFieldOffsetTag,
  ::PROTOBUF_NAMESPACE_ID::internal::kInvalidFieldOffsetTag,
  ::PROTOBUF_NAMESPACE_ID::internal::kInvalidFieldOffsetTag,
  ::PROTOBUF_NAMESPACE_ID::internal::kInvalidFieldOffsetTag,
  ::PROTOBUF_NAMESPACE_ID::internal::kInvalidFieldOffsetTag,
  PROTOBUF_FIELD_OFFSET(::Quix::Process::EventDefinitionProto, level_),
  PROTOBUF_FIELD_OFFSET(::Quix::Process::EventDefinitionProto, location_),
  PROTOBUF_FIELD_OFFSET(::Quix::Process::EventDefinitionProto, name_),
  PROTOBUF_FIELD_OFFSET(::Quix::Process::EventDefinitionProto, description_),
  PROTOBUF_FIELD_OFFSET(::Quix::Process::EventDefinitionProto, customProperties_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::Quix::Process::EventDefinitionsProto, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::Quix::Process::EventDefinitionsProto, events_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::Quix::Process::EventDefinitionProto)},
  { 18, -1, -1, sizeof(::Quix::Process::EventDefinitionsProto)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::Quix::Process::_EventDefinitionProto_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::Quix::Process::_EventDefinitionsProto_default_instance_),
};

const char descriptor_table_protodef_eventDefinitionsProto_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\033eventDefinitionsProto.proto\022\014Quix.Proc"
  "ess\"\225\002\n\024EventDefinitionProto\022\n\n\002id\030\001 \001(\t"
  "\022\023\n\tname_null\030\002 \001(\010H\000\022\024\n\nname_value\030\003 \001("
  "\tH\000\022\032\n\020description_null\030\004 \001(\010H\001\022\033\n\021descr"
  "iption_value\030\005 \001(\tH\001\022\037\n\025customProperties"
  "_null\030\006 \001(\010H\002\022 \n\026customProperties_value\030"
  "\007 \001(\tH\002\022\r\n\005level\030\010 \001(\005\022\020\n\010location\030\t \001(\t"
  "B\006\n\004nameB\r\n\013descriptionB\022\n\020customPropert"
  "ies\"K\n\025EventDefinitionsProto\0222\n\006events\030\001"
  " \003(\0132\".Quix.Process.EventDefinitionProto"
  "B!\252\002\036Quix.Sdk.Process.Models.Codecsb\006pro"
  "to3"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_eventDefinitionsProto_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_eventDefinitionsProto_2eproto = {
  false, false, 443, descriptor_table_protodef_eventDefinitionsProto_2eproto, "eventDefinitionsProto.proto", 
  &descriptor_table_eventDefinitionsProto_2eproto_once, nullptr, 0, 2,
  schemas, file_default_instances, TableStruct_eventDefinitionsProto_2eproto::offsets,
  file_level_metadata_eventDefinitionsProto_2eproto, file_level_enum_descriptors_eventDefinitionsProto_2eproto, file_level_service_descriptors_eventDefinitionsProto_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_eventDefinitionsProto_2eproto_getter() {
  return &descriptor_table_eventDefinitionsProto_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_eventDefinitionsProto_2eproto(&descriptor_table_eventDefinitionsProto_2eproto);
namespace Quix {
namespace Process {

// ===================================================================

class EventDefinitionProto::_Internal {
 public:
};

EventDefinitionProto::EventDefinitionProto(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:Quix.Process.EventDefinitionProto)
}
EventDefinitionProto::EventDefinitionProto(const EventDefinitionProto& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  id_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_id().empty()) {
    id_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_id(), 
      GetArenaForAllocation());
  }
  location_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    location_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_location().empty()) {
    location_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_location(), 
      GetArenaForAllocation());
  }
  level_ = from.level_;
  clear_has_name();
  switch (from.name_case()) {
    case kNameNull: {
      _internal_set_name_null(from._internal_name_null());
      break;
    }
    case kNameValue: {
      _internal_set_name_value(from._internal_name_value());
      break;
    }
    case NAME_NOT_SET: {
      break;
    }
  }
  clear_has_description();
  switch (from.description_case()) {
    case kDescriptionNull: {
      _internal_set_description_null(from._internal_description_null());
      break;
    }
    case kDescriptionValue: {
      _internal_set_description_value(from._internal_description_value());
      break;
    }
    case DESCRIPTION_NOT_SET: {
      break;
    }
  }
  clear_has_customProperties();
  switch (from.customProperties_case()) {
    case kCustomPropertiesNull: {
      _internal_set_customproperties_null(from._internal_customproperties_null());
      break;
    }
    case kCustomPropertiesValue: {
      _internal_set_customproperties_value(from._internal_customproperties_value());
      break;
    }
    case CUSTOMPROPERTIES_NOT_SET: {
      break;
    }
  }
  // @@protoc_insertion_point(copy_constructor:Quix.Process.EventDefinitionProto)
}

inline void EventDefinitionProto::SharedCtor() {
id_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
location_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  location_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
level_ = 0;
clear_has_name();
clear_has_description();
clear_has_customProperties();
}

EventDefinitionProto::~EventDefinitionProto() {
  // @@protoc_insertion_point(destructor:Quix.Process.EventDefinitionProto)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void EventDefinitionProto::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  id_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  location_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (has_name()) {
    clear_name();
  }
  if (has_description()) {
    clear_description();
  }
  if (has_customProperties()) {
    clear_customProperties();
  }
}

void EventDefinitionProto::ArenaDtor(void* object) {
  EventDefinitionProto* _this = reinterpret_cast< EventDefinitionProto* >(object);
  (void)_this;
}
void EventDefinitionProto::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void EventDefinitionProto::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void EventDefinitionProto::clear_name() {
// @@protoc_insertion_point(one_of_clear_start:Quix.Process.EventDefinitionProto)
  switch (name_case()) {
    case kNameNull: {
      // No need to clear
      break;
    }
    case kNameValue: {
      name_.name_value_.Destroy(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
      break;
    }
    case NAME_NOT_SET: {
      break;
    }
  }
  _oneof_case_[0] = NAME_NOT_SET;
}

void EventDefinitionProto::clear_description() {
// @@protoc_insertion_point(one_of_clear_start:Quix.Process.EventDefinitionProto)
  switch (description_case()) {
    case kDescriptionNull: {
      // No need to clear
      break;
    }
    case kDescriptionValue: {
      description_.description_value_.Destroy(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
      break;
    }
    case DESCRIPTION_NOT_SET: {
      break;
    }
  }
  _oneof_case_[1] = DESCRIPTION_NOT_SET;
}

void EventDefinitionProto::clear_customProperties() {
// @@protoc_insertion_point(one_of_clear_start:Quix.Process.EventDefinitionProto)
  switch (customProperties_case()) {
    case kCustomPropertiesNull: {
      // No need to clear
      break;
    }
    case kCustomPropertiesValue: {
      customProperties_.customproperties_value_.Destroy(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
      break;
    }
    case CUSTOMPROPERTIES_NOT_SET: {
      break;
    }
  }
  _oneof_case_[2] = CUSTOMPROPERTIES_NOT_SET;
}


void EventDefinitionProto::Clear() {
// @@protoc_insertion_point(message_clear_start:Quix.Process.EventDefinitionProto)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  id_.ClearToEmpty();
  location_.ClearToEmpty();
  level_ = 0;
  clear_name();
  clear_description();
  clear_customProperties();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* EventDefinitionProto::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // string id = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          auto str = _internal_mutable_id();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "Quix.Process.EventDefinitionProto.id"));
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // bool name_null = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 16)) {
          _internal_set_name_null(::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr));
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // string name_value = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 26)) {
          auto str = _internal_mutable_name_value();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "Quix.Process.EventDefinitionProto.name_value"));
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // bool description_null = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 32)) {
          _internal_set_description_null(::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr));
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // string description_value = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 42)) {
          auto str = _internal_mutable_description_value();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "Quix.Process.EventDefinitionProto.description_value"));
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // bool customProperties_null = 6;
      case 6:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 48)) {
          _internal_set_customproperties_null(::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr));
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // string customProperties_value = 7;
      case 7:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 58)) {
          auto str = _internal_mutable_customproperties_value();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "Quix.Process.EventDefinitionProto.customProperties_value"));
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // int32 level = 8;
      case 8:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 64)) {
          level_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // string location = 9;
      case 9:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 74)) {
          auto str = _internal_mutable_location();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "Quix.Process.EventDefinitionProto.location"));
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* EventDefinitionProto::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:Quix.Process.EventDefinitionProto)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // string id = 1;
  if (!this->_internal_id().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_id().data(), static_cast<int>(this->_internal_id().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "Quix.Process.EventDefinitionProto.id");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_id(), target);
  }

  // bool name_null = 2;
  if (_internal_has_name_null()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteBoolToArray(2, this->_internal_name_null(), target);
  }

  // string name_value = 3;
  if (_internal_has_name_value()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_name_value().data(), static_cast<int>(this->_internal_name_value().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "Quix.Process.EventDefinitionProto.name_value");
    target = stream->WriteStringMaybeAliased(
        3, this->_internal_name_value(), target);
  }

  // bool description_null = 4;
  if (_internal_has_description_null()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteBoolToArray(4, this->_internal_description_null(), target);
  }

  // string description_value = 5;
  if (_internal_has_description_value()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_description_value().data(), static_cast<int>(this->_internal_description_value().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "Quix.Process.EventDefinitionProto.description_value");
    target = stream->WriteStringMaybeAliased(
        5, this->_internal_description_value(), target);
  }

  // bool customProperties_null = 6;
  if (_internal_has_customproperties_null()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteBoolToArray(6, this->_internal_customproperties_null(), target);
  }

  // string customProperties_value = 7;
  if (_internal_has_customproperties_value()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_customproperties_value().data(), static_cast<int>(this->_internal_customproperties_value().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "Quix.Process.EventDefinitionProto.customProperties_value");
    target = stream->WriteStringMaybeAliased(
        7, this->_internal_customproperties_value(), target);
  }

  // int32 level = 8;
  if (this->_internal_level() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(8, this->_internal_level(), target);
  }

  // string location = 9;
  if (!this->_internal_location().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_location().data(), static_cast<int>(this->_internal_location().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "Quix.Process.EventDefinitionProto.location");
    target = stream->WriteStringMaybeAliased(
        9, this->_internal_location(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Quix.Process.EventDefinitionProto)
  return target;
}

size_t EventDefinitionProto::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:Quix.Process.EventDefinitionProto)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string id = 1;
  if (!this->_internal_id().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_id());
  }

  // string location = 9;
  if (!this->_internal_location().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_location());
  }

  // int32 level = 8;
  if (this->_internal_level() != 0) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32SizePlusOne(this->_internal_level());
  }

  switch (name_case()) {
    // bool name_null = 2;
    case kNameNull: {
      total_size += 1 + 1;
      break;
    }
    // string name_value = 3;
    case kNameValue: {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
          this->_internal_name_value());
      break;
    }
    case NAME_NOT_SET: {
      break;
    }
  }
  switch (description_case()) {
    // bool description_null = 4;
    case kDescriptionNull: {
      total_size += 1 + 1;
      break;
    }
    // string description_value = 5;
    case kDescriptionValue: {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
          this->_internal_description_value());
      break;
    }
    case DESCRIPTION_NOT_SET: {
      break;
    }
  }
  switch (customProperties_case()) {
    // bool customProperties_null = 6;
    case kCustomPropertiesNull: {
      total_size += 1 + 1;
      break;
    }
    // string customProperties_value = 7;
    case kCustomPropertiesValue: {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
          this->_internal_customproperties_value());
      break;
    }
    case CUSTOMPROPERTIES_NOT_SET: {
      break;
    }
  }
  return MaybeComputeUnknownFieldsSize(total_size, &_cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData EventDefinitionProto::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    EventDefinitionProto::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*EventDefinitionProto::GetClassData() const { return &_class_data_; }

void EventDefinitionProto::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to,
                      const ::PROTOBUF_NAMESPACE_ID::Message& from) {
  static_cast<EventDefinitionProto *>(to)->MergeFrom(
      static_cast<const EventDefinitionProto &>(from));
}


void EventDefinitionProto::MergeFrom(const EventDefinitionProto& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:Quix.Process.EventDefinitionProto)
  GOOGLE_DCHECK_NE(&from, this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_id().empty()) {
    _internal_set_id(from._internal_id());
  }
  if (!from._internal_location().empty()) {
    _internal_set_location(from._internal_location());
  }
  if (from._internal_level() != 0) {
    _internal_set_level(from._internal_level());
  }
  switch (from.name_case()) {
    case kNameNull: {
      _internal_set_name_null(from._internal_name_null());
      break;
    }
    case kNameValue: {
      _internal_set_name_value(from._internal_name_value());
      break;
    }
    case NAME_NOT_SET: {
      break;
    }
  }
  switch (from.description_case()) {
    case kDescriptionNull: {
      _internal_set_description_null(from._internal_description_null());
      break;
    }
    case kDescriptionValue: {
      _internal_set_description_value(from._internal_description_value());
      break;
    }
    case DESCRIPTION_NOT_SET: {
      break;
    }
  }
  switch (from.customProperties_case()) {
    case kCustomPropertiesNull: {
      _internal_set_customproperties_null(from._internal_customproperties_null());
      break;
    }
    case kCustomPropertiesValue: {
      _internal_set_customproperties_value(from._internal_customproperties_value());
      break;
    }
    case CUSTOMPROPERTIES_NOT_SET: {
      break;
    }
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void EventDefinitionProto::CopyFrom(const EventDefinitionProto& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Quix.Process.EventDefinitionProto)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool EventDefinitionProto::IsInitialized() const {
  return true;
}

void EventDefinitionProto::InternalSwap(EventDefinitionProto* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      &id_, lhs_arena,
      &other->id_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      &location_, lhs_arena,
      &other->location_, rhs_arena
  );
  swap(level_, other->level_);
  swap(name_, other->name_);
  swap(description_, other->description_);
  swap(customProperties_, other->customProperties_);
  swap(_oneof_case_[0], other->_oneof_case_[0]);
  swap(_oneof_case_[1], other->_oneof_case_[1]);
  swap(_oneof_case_[2], other->_oneof_case_[2]);
}

::PROTOBUF_NAMESPACE_ID::Metadata EventDefinitionProto::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_eventDefinitionsProto_2eproto_getter, &descriptor_table_eventDefinitionsProto_2eproto_once,
      file_level_metadata_eventDefinitionsProto_2eproto[0]);
}

// ===================================================================

class EventDefinitionsProto::_Internal {
 public:
};

EventDefinitionsProto::EventDefinitionsProto(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned),
  events_(arena) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:Quix.Process.EventDefinitionsProto)
}
EventDefinitionsProto::EventDefinitionsProto(const EventDefinitionsProto& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      events_(from.events_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:Quix.Process.EventDefinitionsProto)
}

inline void EventDefinitionsProto::SharedCtor() {
}

EventDefinitionsProto::~EventDefinitionsProto() {
  // @@protoc_insertion_point(destructor:Quix.Process.EventDefinitionsProto)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void EventDefinitionsProto::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void EventDefinitionsProto::ArenaDtor(void* object) {
  EventDefinitionsProto* _this = reinterpret_cast< EventDefinitionsProto* >(object);
  (void)_this;
}
void EventDefinitionsProto::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void EventDefinitionsProto::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void EventDefinitionsProto::Clear() {
// @@protoc_insertion_point(message_clear_start:Quix.Process.EventDefinitionsProto)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  events_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* EventDefinitionsProto::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // repeated .Quix.Process.EventDefinitionProto events = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_events(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<10>(ptr));
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* EventDefinitionsProto::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:Quix.Process.EventDefinitionsProto)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated .Quix.Process.EventDefinitionProto events = 1;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->_internal_events_size()); i < n; i++) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(1, this->_internal_events(i), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Quix.Process.EventDefinitionsProto)
  return target;
}

size_t EventDefinitionsProto::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:Quix.Process.EventDefinitionsProto)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated .Quix.Process.EventDefinitionProto events = 1;
  total_size += 1UL * this->_internal_events_size();
  for (const auto& msg : this->events_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData EventDefinitionsProto::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    EventDefinitionsProto::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*EventDefinitionsProto::GetClassData() const { return &_class_data_; }

void EventDefinitionsProto::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to,
                      const ::PROTOBUF_NAMESPACE_ID::Message& from) {
  static_cast<EventDefinitionsProto *>(to)->MergeFrom(
      static_cast<const EventDefinitionsProto &>(from));
}


void EventDefinitionsProto::MergeFrom(const EventDefinitionsProto& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:Quix.Process.EventDefinitionsProto)
  GOOGLE_DCHECK_NE(&from, this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  events_.MergeFrom(from.events_);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void EventDefinitionsProto::CopyFrom(const EventDefinitionsProto& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Quix.Process.EventDefinitionsProto)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool EventDefinitionsProto::IsInitialized() const {
  return true;
}

void EventDefinitionsProto::InternalSwap(EventDefinitionsProto* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  events_.InternalSwap(&other->events_);
}

::PROTOBUF_NAMESPACE_ID::Metadata EventDefinitionsProto::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_eventDefinitionsProto_2eproto_getter, &descriptor_table_eventDefinitionsProto_2eproto_once,
      file_level_metadata_eventDefinitionsProto_2eproto[1]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace Process
}  // namespace Quix
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::Quix::Process::EventDefinitionProto* Arena::CreateMaybeMessage< ::Quix::Process::EventDefinitionProto >(Arena* arena) {
  return Arena::CreateMessageInternal< ::Quix::Process::EventDefinitionProto >(arena);
}
template<> PROTOBUF_NOINLINE ::Quix::Process::EventDefinitionsProto* Arena::CreateMaybeMessage< ::Quix::Process::EventDefinitionsProto >(Arena* arena) {
  return Arena::CreateMessageInternal< ::Quix::Process::EventDefinitionsProto >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
