[IBM Server Implmentation](https://www.ibm.com/docs/en/i/7.1?topic=designs-using-poll-instead-select)

```cpp

poll_fd array pollfd[200] //can change to vector
init_server fd = 3

pollfd[0] = 3 s
pollfd[1] = 4 c

if (poll(pollfd, poll_len, timeout))
{
	while (i < poll_num)
	{
		if (err)
			bad;

		if (nothing)
			continue;

		if (server)
		{
			if (something happens (revents))
			{
				client_fd = accept connection
				add client fd to pollfd => pollfd[1] = client_fd
				set pollfd[1].events = POLLIN;
			}
			else
				do nothing, continue;
		}
		if (client)
		{
			if (something happened and client is SEND_MODE / POLLIN)
			{
				read_header / read_body
				when done header -> pollfd[1].events = POLLOUT
			}
			else if (something happened and client is in READ_MOD / POLLOUT)
			{
				write to client;
				when done send -> close connection = close fd
			}
		}
	}
}
```

`client1 [5] -> server1 [3]`
`client2 [6] -> server2 [4]`

```cpp
class HTTP_SEREVER
{
	server_poll;

	map<serverfd, ServerClass>
}


class ServerClass
{
	accept_connection;

	map<client_fd, ClientClass>
}

class ClientClass
{
	server_fd;
	Request;
	Response;
}```
