#include <stdio.h>
#include <signal.h>
#include "../Socket/ListeningSocket.hpp"

namespace FT {

class SimpleServer
{
	private:
		ListeningSocket *listeningSocket;
		bool keeprunning;

		// Define pure virtual functions, which makes this class abstract
		virtual void	parseConfig() = 0;
		virtual void	accepter() = 0;
		virtual void	handler() = 0;
		virtual void	responder() = 0;

	protected:
		void			registerSignal();

	public:
		SimpleServer(int domain, int service, int protocol, int port, u_long interface, int backlog);
		virtual ~SimpleServer();

		void launch();
		void setKeeprunning(bool keeprunning);
		ListeningSocket * get_socket();
};

}
