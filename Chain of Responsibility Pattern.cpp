#include<iostream>
#include<vector>
#include<fstream>

class ErrorsHandler
{
public:
	virtual void setNextHandler(ErrorsHandler* handler) = 0;
	virtual void handleRequest(const std::string& request) = 0;
};


class FatalErrorHandler :public ErrorsHandler {
private:
	ErrorsHandler* nextHandler;
public:
	void setNextHandler(ErrorsHandler* handler) override
	{
		nextHandler = handler;
	}
	void handleRequest(const std::string& request) override
	{
		if (request == "Fatal_error")
		{
			throw "fatal_error";
		}
		else if (nextHandler != nullptr)
		{
			nextHandler->handleRequest(request);
		}
	}
};

class ErrorHandler :public ErrorsHandler {
public:
	ErrorHandler(std::string f) : file_name(f) {}

	void setNextHandler(ErrorsHandler* handler) override
	{
		nextHandler = handler;
	}
	void handleRequest(const std::string& request) override
	{
		if (request == "Error")
		{
			std::ofstream out;
			out.open(file_name);
			out << "Error" << std::endl;

		}
		else if (nextHandler != nullptr)
		{
			nextHandler->handleRequest(request);
		}
	}
private:
	ErrorsHandler* nextHandler;
	std::string file_name;
};

class WarningHandler :public ErrorsHandler {
private:
	ErrorsHandler* nextHandler;

public:
	void setNextHandler(ErrorsHandler* handler) override
	{
		nextHandler = handler;
	}
	void handleRequest(const std::string& request) override
	{
		if (request == "Warning")
		{
			std::cout << "Warning" << std::endl;
		}
		else if (nextHandler != nullptr)
		{
			nextHandler->handleRequest(request);
		}
	}
};

class UnknownHandler :public ErrorsHandler {
private:
	ErrorsHandler* nextHandler;

public:
	void setNextHandler(ErrorsHandler* handler) override
	{
		nextHandler = handler;
	}
	void handleRequest(const std::string& request) override
	{
		if (request == "Unknown")
		{
			throw "message unprocessed";
		}
		else if (nextHandler != nullptr)
		{
			nextHandler->handleRequest(request);
		}
	}
};


int main()
{
	try {
		ErrorsHandler* fatal_Handler = new FatalErrorHandler();
		ErrorsHandler* error_handler = new ErrorHandler("file.log");
		ErrorsHandler* warning_handler = new WarningHandler();
		ErrorsHandler* unknown_handler = new UnknownHandler();

		fatal_Handler->setNextHandler(error_handler);

		error_handler->setNextHandler(warning_handler);

		warning_handler->setNextHandler(unknown_handler);

		fatal_Handler->handleRequest("Fatal_error");
		

		delete fatal_Handler;
		delete error_handler;
		delete warning_handler;
		delete unknown_handler;
	}
	catch (const char* ex) {
	std::cout << ex;
	}
	
	return 0;
}
