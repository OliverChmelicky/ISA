//
// Created by olo on 31.10.19.
//

#ifndef ISA_ERRORS_H
#define ISA_ERRORS_H


class errors {
public:
	static int sendErrorNotFound(int socket);
	static int sendErrorBadRequest(int socket);
	static int sendErrorExists(int socket);
};


#endif //ISA_ERRORS_H
