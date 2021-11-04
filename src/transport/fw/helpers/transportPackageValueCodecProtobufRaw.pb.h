// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: transportPackageValueCodecProtobufRaw.proto

#ifndef PROTOBUF_INCLUDED_transportPackageValueCodecProtobufRaw_2eproto
#define PROTOBUF_INCLUDED_transportPackageValueCodecProtobufRaw_2eproto

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
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/map.h>  // IWYU pragma: export
#include <google/protobuf/map_entry.h>
#include <google/protobuf/map_field_inl.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_transportPackageValueCodecProtobufRaw_2eproto 

namespace protobuf_transportPackageValueCodecProtobufRaw_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[2];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_transportPackageValueCodecProtobufRaw_2eproto
namespace Quix {
namespace Transport {
class TransportPackageValueCodecProtobufRaw;
class TransportPackageValueCodecProtobufRawDefaultTypeInternal;
extern TransportPackageValueCodecProtobufRawDefaultTypeInternal _TransportPackageValueCodecProtobufRaw_default_instance_;
class TransportPackageValueCodecProtobufRaw_MetadataEntry_DoNotUse;
class TransportPackageValueCodecProtobufRaw_MetadataEntry_DoNotUseDefaultTypeInternal;
extern TransportPackageValueCodecProtobufRaw_MetadataEntry_DoNotUseDefaultTypeInternal _TransportPackageValueCodecProtobufRaw_MetadataEntry_DoNotUse_default_instance_;
}  // namespace Transport
}  // namespace Quix
namespace google {
namespace protobuf {
template<> ::Quix::Transport::TransportPackageValueCodecProtobufRaw* Arena::CreateMaybeMessage<::Quix::Transport::TransportPackageValueCodecProtobufRaw>(Arena*);
template<> ::Quix::Transport::TransportPackageValueCodecProtobufRaw_MetadataEntry_DoNotUse* Arena::CreateMaybeMessage<::Quix::Transport::TransportPackageValueCodecProtobufRaw_MetadataEntry_DoNotUse>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace Quix {
namespace Transport {

// ===================================================================

class TransportPackageValueCodecProtobufRaw_MetadataEntry_DoNotUse : public ::google::protobuf::internal::MapEntry<TransportPackageValueCodecProtobufRaw_MetadataEntry_DoNotUse, 
    ::std::string, ::std::string,
    ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
    ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
    0 > {
public:
  typedef ::google::protobuf::internal::MapEntry<TransportPackageValueCodecProtobufRaw_MetadataEntry_DoNotUse, 
    ::std::string, ::std::string,
    ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
    ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
    0 > SuperType;
  TransportPackageValueCodecProtobufRaw_MetadataEntry_DoNotUse();
  TransportPackageValueCodecProtobufRaw_MetadataEntry_DoNotUse(::google::protobuf::Arena* arena);
  void MergeFrom(const TransportPackageValueCodecProtobufRaw_MetadataEntry_DoNotUse& other);
  static const TransportPackageValueCodecProtobufRaw_MetadataEntry_DoNotUse* internal_default_instance() { return reinterpret_cast<const TransportPackageValueCodecProtobufRaw_MetadataEntry_DoNotUse*>(&_TransportPackageValueCodecProtobufRaw_MetadataEntry_DoNotUse_default_instance_); }
  void MergeFrom(const ::google::protobuf::Message& other) final;
  ::google::protobuf::Metadata GetMetadata() const;
};

// -------------------------------------------------------------------

class TransportPackageValueCodecProtobufRaw : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Quix.Transport.TransportPackageValueCodecProtobufRaw) */ {
 public:
  TransportPackageValueCodecProtobufRaw();
  virtual ~TransportPackageValueCodecProtobufRaw();

  TransportPackageValueCodecProtobufRaw(const TransportPackageValueCodecProtobufRaw& from);

  inline TransportPackageValueCodecProtobufRaw& operator=(const TransportPackageValueCodecProtobufRaw& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  TransportPackageValueCodecProtobufRaw(TransportPackageValueCodecProtobufRaw&& from) noexcept
    : TransportPackageValueCodecProtobufRaw() {
    *this = ::std::move(from);
  }

  inline TransportPackageValueCodecProtobufRaw& operator=(TransportPackageValueCodecProtobufRaw&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const TransportPackageValueCodecProtobufRaw& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const TransportPackageValueCodecProtobufRaw* internal_default_instance() {
    return reinterpret_cast<const TransportPackageValueCodecProtobufRaw*>(
               &_TransportPackageValueCodecProtobufRaw_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(TransportPackageValueCodecProtobufRaw* other);
  friend void swap(TransportPackageValueCodecProtobufRaw& a, TransportPackageValueCodecProtobufRaw& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline TransportPackageValueCodecProtobufRaw* New() const final {
    return CreateMaybeMessage<TransportPackageValueCodecProtobufRaw>(NULL);
  }

  TransportPackageValueCodecProtobufRaw* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<TransportPackageValueCodecProtobufRaw>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const TransportPackageValueCodecProtobufRaw& from);
  void MergeFrom(const TransportPackageValueCodecProtobufRaw& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(TransportPackageValueCodecProtobufRaw* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------


  // accessors -------------------------------------------------------

  // map<string, string> metadata = 3;
  int metadata_size() const;
  void clear_metadata();
  static const int kMetadataFieldNumber = 3;
  const ::google::protobuf::Map< ::std::string, ::std::string >&
      metadata() const;
  ::google::protobuf::Map< ::std::string, ::std::string >*
      mutable_metadata();

  // string codecId = 1;
  void clear_codecid();
  static const int kCodecIdFieldNumber = 1;
  const ::std::string& codecid() const;
  void set_codecid(const ::std::string& value);
  #if LANG_CXX11
  void set_codecid(::std::string&& value);
  #endif
  void set_codecid(const char* value);
  void set_codecid(const char* value, size_t size);
  ::std::string* mutable_codecid();
  ::std::string* release_codecid();
  void set_allocated_codecid(::std::string* codecid);

  // string modelKey = 2;
  void clear_modelkey();
  static const int kModelKeyFieldNumber = 2;
  const ::std::string& modelkey() const;
  void set_modelkey(const ::std::string& value);
  #if LANG_CXX11
  void set_modelkey(::std::string&& value);
  #endif
  void set_modelkey(const char* value);
  void set_modelkey(const char* value, size_t size);
  ::std::string* mutable_modelkey();
  ::std::string* release_modelkey();
  void set_allocated_modelkey(::std::string* modelkey);

  // bytes data = 4;
  void clear_data();
  static const int kDataFieldNumber = 4;
  const ::std::string& data() const;
  void set_data(const ::std::string& value);
  #if LANG_CXX11
  void set_data(::std::string&& value);
  #endif
  void set_data(const char* value);
  void set_data(const void* value, size_t size);
  ::std::string* mutable_data();
  ::std::string* release_data();
  void set_allocated_data(::std::string* data);

  // @@protoc_insertion_point(class_scope:Quix.Transport.TransportPackageValueCodecProtobufRaw)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::MapField<
      TransportPackageValueCodecProtobufRaw_MetadataEntry_DoNotUse,
      ::std::string, ::std::string,
      ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
      ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
      0 > metadata_;
  ::google::protobuf::internal::ArenaStringPtr codecid_;
  ::google::protobuf::internal::ArenaStringPtr modelkey_;
  ::google::protobuf::internal::ArenaStringPtr data_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_transportPackageValueCodecProtobufRaw_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// -------------------------------------------------------------------

// TransportPackageValueCodecProtobufRaw

// string codecId = 1;
inline void TransportPackageValueCodecProtobufRaw::clear_codecid() {
  codecid_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& TransportPackageValueCodecProtobufRaw::codecid() const {
  // @@protoc_insertion_point(field_get:Quix.Transport.TransportPackageValueCodecProtobufRaw.codecId)
  return codecid_.GetNoArena();
}
inline void TransportPackageValueCodecProtobufRaw::set_codecid(const ::std::string& value) {
  
  codecid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Quix.Transport.TransportPackageValueCodecProtobufRaw.codecId)
}
#if LANG_CXX11
inline void TransportPackageValueCodecProtobufRaw::set_codecid(::std::string&& value) {
  
  codecid_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:Quix.Transport.TransportPackageValueCodecProtobufRaw.codecId)
}
#endif
inline void TransportPackageValueCodecProtobufRaw::set_codecid(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  codecid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Quix.Transport.TransportPackageValueCodecProtobufRaw.codecId)
}
inline void TransportPackageValueCodecProtobufRaw::set_codecid(const char* value, size_t size) {
  
  codecid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Quix.Transport.TransportPackageValueCodecProtobufRaw.codecId)
}
inline ::std::string* TransportPackageValueCodecProtobufRaw::mutable_codecid() {
  
  // @@protoc_insertion_point(field_mutable:Quix.Transport.TransportPackageValueCodecProtobufRaw.codecId)
  return codecid_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* TransportPackageValueCodecProtobufRaw::release_codecid() {
  // @@protoc_insertion_point(field_release:Quix.Transport.TransportPackageValueCodecProtobufRaw.codecId)
  
  return codecid_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TransportPackageValueCodecProtobufRaw::set_allocated_codecid(::std::string* codecid) {
  if (codecid != NULL) {
    
  } else {
    
  }
  codecid_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), codecid);
  // @@protoc_insertion_point(field_set_allocated:Quix.Transport.TransportPackageValueCodecProtobufRaw.codecId)
}

// string modelKey = 2;
inline void TransportPackageValueCodecProtobufRaw::clear_modelkey() {
  modelkey_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& TransportPackageValueCodecProtobufRaw::modelkey() const {
  // @@protoc_insertion_point(field_get:Quix.Transport.TransportPackageValueCodecProtobufRaw.modelKey)
  return modelkey_.GetNoArena();
}
inline void TransportPackageValueCodecProtobufRaw::set_modelkey(const ::std::string& value) {
  
  modelkey_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Quix.Transport.TransportPackageValueCodecProtobufRaw.modelKey)
}
#if LANG_CXX11
inline void TransportPackageValueCodecProtobufRaw::set_modelkey(::std::string&& value) {
  
  modelkey_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:Quix.Transport.TransportPackageValueCodecProtobufRaw.modelKey)
}
#endif
inline void TransportPackageValueCodecProtobufRaw::set_modelkey(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  modelkey_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Quix.Transport.TransportPackageValueCodecProtobufRaw.modelKey)
}
inline void TransportPackageValueCodecProtobufRaw::set_modelkey(const char* value, size_t size) {
  
  modelkey_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Quix.Transport.TransportPackageValueCodecProtobufRaw.modelKey)
}
inline ::std::string* TransportPackageValueCodecProtobufRaw::mutable_modelkey() {
  
  // @@protoc_insertion_point(field_mutable:Quix.Transport.TransportPackageValueCodecProtobufRaw.modelKey)
  return modelkey_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* TransportPackageValueCodecProtobufRaw::release_modelkey() {
  // @@protoc_insertion_point(field_release:Quix.Transport.TransportPackageValueCodecProtobufRaw.modelKey)
  
  return modelkey_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TransportPackageValueCodecProtobufRaw::set_allocated_modelkey(::std::string* modelkey) {
  if (modelkey != NULL) {
    
  } else {
    
  }
  modelkey_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), modelkey);
  // @@protoc_insertion_point(field_set_allocated:Quix.Transport.TransportPackageValueCodecProtobufRaw.modelKey)
}

// map<string, string> metadata = 3;
inline int TransportPackageValueCodecProtobufRaw::metadata_size() const {
  return metadata_.size();
}
inline void TransportPackageValueCodecProtobufRaw::clear_metadata() {
  metadata_.Clear();
}
inline const ::google::protobuf::Map< ::std::string, ::std::string >&
TransportPackageValueCodecProtobufRaw::metadata() const {
  // @@protoc_insertion_point(field_map:Quix.Transport.TransportPackageValueCodecProtobufRaw.metadata)
  return metadata_.GetMap();
}
inline ::google::protobuf::Map< ::std::string, ::std::string >*
TransportPackageValueCodecProtobufRaw::mutable_metadata() {
  // @@protoc_insertion_point(field_mutable_map:Quix.Transport.TransportPackageValueCodecProtobufRaw.metadata)
  return metadata_.MutableMap();
}

// bytes data = 4;
inline void TransportPackageValueCodecProtobufRaw::clear_data() {
  data_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& TransportPackageValueCodecProtobufRaw::data() const {
  // @@protoc_insertion_point(field_get:Quix.Transport.TransportPackageValueCodecProtobufRaw.data)
  return data_.GetNoArena();
}
inline void TransportPackageValueCodecProtobufRaw::set_data(const ::std::string& value) {
  
  data_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Quix.Transport.TransportPackageValueCodecProtobufRaw.data)
}
#if LANG_CXX11
inline void TransportPackageValueCodecProtobufRaw::set_data(::std::string&& value) {
  
  data_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:Quix.Transport.TransportPackageValueCodecProtobufRaw.data)
}
#endif
inline void TransportPackageValueCodecProtobufRaw::set_data(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  data_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Quix.Transport.TransportPackageValueCodecProtobufRaw.data)
}
inline void TransportPackageValueCodecProtobufRaw::set_data(const void* value, size_t size) {
  
  data_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Quix.Transport.TransportPackageValueCodecProtobufRaw.data)
}
inline ::std::string* TransportPackageValueCodecProtobufRaw::mutable_data() {
  
  // @@protoc_insertion_point(field_mutable:Quix.Transport.TransportPackageValueCodecProtobufRaw.data)
  return data_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* TransportPackageValueCodecProtobufRaw::release_data() {
  // @@protoc_insertion_point(field_release:Quix.Transport.TransportPackageValueCodecProtobufRaw.data)
  
  return data_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TransportPackageValueCodecProtobufRaw::set_allocated_data(::std::string* data) {
  if (data != NULL) {
    
  } else {
    
  }
  data_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), data);
  // @@protoc_insertion_point(field_set_allocated:Quix.Transport.TransportPackageValueCodecProtobufRaw.data)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace Transport
}  // namespace Quix

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_transportPackageValueCodecProtobufRaw_2eproto