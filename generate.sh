#!/bin/sh
C:\Users\onuru\OneDrive\Desktop\protoc -I=proto --python_out=python --c_out=c --cpp_out=cpp --doc_out=docs C:/Users/onuru/OneDrive/Desktop/warp-firmware/proto/warp_protocol.proto

nanopb_generator C:/Users/onuru/OneDrive/Desktop/warp-firmware/proto/warp_protocol.proto --output-dir=nanopb