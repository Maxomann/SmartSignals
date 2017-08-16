#pragma once
#include <functional>
#include <memory>
#include <vector>
#include <list>
#include <utility>
#include <optional>
#include <algorithm>

namespace smartsignals
{

	using ConnectionId = size_t;

	template< class T >
	class Signal;

	template< class RetVal, class ... Args >
	class Signal<RetVal( Args... )>
	{
	private:
		ConnectionId connectionCounter = 0;

		std::list<std::pair<ConnectionId, std::function<RetVal(Args ...)>>> connectedElements;

	public:
		ConnectionId connect(std::function<RetVal(Args ...)> function)
		{
			ConnectionId connectionId = connectionCounter;
			connectionCounter++;

			connectedElements.push_back(
				std::make_pair(
					connectionId,
					std::move(function)
				)
			);

			return connectionId;
		};

		void disconnect(ConnectionId connectionToRemove)
		{
			connectedElements.erase( std::find_if( connectedElements.begin(), connectedElements.end(), [&](auto& el) { return (el.first == connectionToRemove)} ) );
		}

		void disconnect(std::vector<ConnectionId> connectionsToRemove)
		{
			/*...*/
		}

		bool containsConnection(ConnectionId connection)
		{
			return (std::find_if(connectedElements.begin(), connectedElements.end(), [&](auto& el) { return (el.first == connectionToRemove)}) != connectedElements.end())
		}

		void reset()
		{
			/*...*/
		}

		void call(Args&& ...)const
		{
			// copy elements to enable adding/removing elements in recursive calls
			std::vector<std::function<RetVal(Args ...)>> toExecute;

			for (const auto& el : connectedElements)
				toExecute.push_back(el.second);

			for (const auto& el : toExecute)
				el( (std::forward<Args>(args)...) );
		};
	};

	class Connections
	{
		/*...*/
	};
}
