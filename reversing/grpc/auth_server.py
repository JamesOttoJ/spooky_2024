# Copyright 2015 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
"""The Python implementation of the GRPC helloworld.Greeter server."""

from concurrent import futures
import logging

import grpc
auth_service_pb2, auth_service_pb2_grpc = grpc.protos_and_services(
    "auth.proto"
)

_LOGGER = logging.getLogger(__name__)
_LOGGER.setLevel(logging.INFO)

_LISTEN_ADDRESS_TEMPLATE = "localhost:%d"
_AUTH_HEADER_KEY = "authorization"
_AUTH_HEADER_VALUE = "Bearer example_oauth2_token"

class SignatureValidationInterceptor(grpc.ServerInterceptor):
    def __init__(self):
        print("sig init")
        def abort(ignored_request, context):
            print("sig abort")
            context.abort(grpc.StatusCode.UNAUTHENTICATED, "Invalid signature")

        self._abort_handler = grpc.unary_unary_rpc_method_handler(abort)

    def intercept_service(self, continuation, handler_call_details):
        print("sig intercept")
        print(handler_call_details)
        # Example HandlerCallDetails object:
        #     _HandlerCallDetails(
        #       method=u'/auth_service.AuthService/Ping',
        #       invocation_metadata=...)
        # expected_metadata = (_AUTH_HEADER_KEY, _AUTH_HEADER_VALUE)
        # if expected_metadata in handler_call_details.invocation_metadata:
        # if '/auth_service.AuthService/Ping' in handler_call_details.method:
        return continuation(handler_call_details)
            # context.abort(grpc.StatusCode.OK, "Correct Method")
        # else:
        #     return self._abort_handler

class AuthService(auth_service_pb2_grpc.AuthServiceServicer):
    def Ping(self, request, context):
        print("Ping data:")
        print(request)
        return auth_service_pb2.PingResponse(response=1)
    
    def Login(self, request, context):
        return auth_service_pb2.LoginResponse(response=True)

    def Logout(self, request, context):
        return auth_service_pb2.LogoutResponse(response=True)

    def Hidden(self, request, context):
        return auth_service_pb2.HiddenResponse(flag="sp00ky{7haNk5_F0r_tH3_c4Ll}")

def serve():
    port = "50052"
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10),
            interceptors=(SignatureValidationInterceptor(),),
    )
    auth_service_pb2_grpc.add_AuthServiceServicer_to_server(AuthService(), server)
    server.add_insecure_port("localhost:" + port)
    server.start()
    print("Server started, listening on " + port)
    server.wait_for_termination()


if __name__ == "__main__":
    logging.basicConfig()
    serve()
