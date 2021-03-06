# scintilla-arduino-projects

## Protobufs

### 1) Download and install protobufs 2.6.1 from Google

https://github.com/google/protobuf/releases/tag/v2.6.1

```
./configure && make && make install
```

### 2) Download NanoPB

http://koti.kapsi.fi/jpa/nanopb/download/

### 3) Create your .proto file

```
// A very simple protocol definition, consisting of only
// one message.

syntax = "proto2";

message SimpleMessage {
    required int32 lucky_number = 1;
}
```

### 4) Compile your .proto with NanoPB protoc

/path/to/nanopb/generator-bin/protoc -I=/path/to/your/.proto/file's/folder/ --nanopb_out=/path/to/output/folder/ /path/to/your/.proto/file's/folder/your-proto-file.proto

### 5) Add the generated .c and .h files to your Arduino project directory

### 6) Add NanoPB to your Arduino libraries folder

### 7) Include the required NanoPB files

```
#include <pb_common.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include <pb.h>
```

### 8) Include your generated .h file

```
#include "your-proto.pb.h"
```

### 9) Use your protobuf!

