namespace ft{
	class Client {
		public:
			Client(){};
			~Client() {};
			Client(int fd) : server_fd(fd) {}
		private:
			int	server_fd;
	};
}