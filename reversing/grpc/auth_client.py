import argparse
import asyncio
import logging

import _credentials
import grpc


auth_service_pb2, auth_service_pb2_grpc = grpc.protos_and_services(
    "auth.proto"
)

def run():
    # metadata = ('user-agent', 'grpc-go/1.64.0')
    with grpc.insecure_channel("localhost:50052") as channel:
        auth_stub = auth_service_pb2_grpc.AuthServiceStub(channel)
        response = auth_stub.Hidden(auth_service_pb2.HiddenRequest())
        print("Received: " + str(response.flag))

if __name__ == "__main__":
    logging.basicConfig()
    run()
