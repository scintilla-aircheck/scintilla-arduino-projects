# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: sensor_data.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='sensor_data.proto',
  package='',
  serialized_pb=_b('\n\x11sensor_data.proto\")\n\x11SensorDataMessage\x12\x14\n\x0clucky_number\x18\x01 \x02(\x05')
)
_sym_db.RegisterFileDescriptor(DESCRIPTOR)




_SENSORDATAMESSAGE = _descriptor.Descriptor(
  name='SensorDataMessage',
  full_name='SensorDataMessage',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='lucky_number', full_name='SensorDataMessage.lucky_number', index=0,
      number=1, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=21,
  serialized_end=62,
)

DESCRIPTOR.message_types_by_name['SensorDataMessage'] = _SENSORDATAMESSAGE

SensorDataMessage = _reflection.GeneratedProtocolMessageType('SensorDataMessage', (_message.Message,), dict(
  DESCRIPTOR = _SENSORDATAMESSAGE,
  __module__ = 'sensor_data_pb2'
  # @@protoc_insertion_point(class_scope:SensorDataMessage)
  ))
_sym_db.RegisterMessage(SensorDataMessage)


# @@protoc_insertion_point(module_scope)