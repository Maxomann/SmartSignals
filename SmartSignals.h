#pragma once
#include <functional>
#include <memory>
#include <vector>
#include <utility>
#include <optional>
#include <algorithm>

namespace smartsignals
{
	using ConnectionId = size_t;

	class SignalBase
	{
		std::shared_ptr<bool> state = std::make_shared<bool>(true);

	public:
		virtual void disconnect(const ConnectionId& connectionToRemove) = 0;

		virtual void disconnect(const std::vector<ConnectionId>& connectionsToRemove) = 0;

		virtual bool isConnectionIdValid(const ConnectionId& connection)const = 0;

		std::weak_ptr<bool> getStateTrackerPtr()const
		{
			return state;
		}
	};

	class ConnectionReference
	{
		SignalBase& signalRef;
		std::weak_ptr<bool> signalState;

		const ConnectionId connectionId;

	public:
		ConnectionReference(SignalBase& signalRef, const ConnectionId connectionId)
			: signalRef(signalRef),
			signalState(signalRef.getStateTrackerPtr()),
			connectionId(connectionId)
		{

		}

		bool isValid()const
		{
			return (!signalState.expired() && signalRef.isConnectionIdValid(connectionId));
		}

		void disconnect()const
		{
			if (isValid())
				signalRef.disconnect(connectionId);
		}
	};

	class ConnectionTracker
	{
		std::vector<ConnectionReference> connections;

	public:
		void trackConnection(ConnectionReference ref)
		{
			connections.push_back(std::move(ref));
		}

		void disconnectAll()
		{
			for (const auto& el : connections)
				el.disconnect();
			connections.clear();
		}
	};

	template< class T >
	class Signal;

	template< class RetVal, class ... Args >
	class Signal<RetVal( Args... )> : public SignalBase
	{
	private:
		ConnectionId connectionIdCounter = 0;

		std::vector<std::pair<ConnectionId, std::function<RetVal(Args ...)>>> connectedElements;

	public:
		ConnectionId connect(std::function<RetVal(Args ...)> function)
		{
			ConnectionId connectionId = connectionIdCounter;
			connectionIdCounter++;

			connectedElements.push_back(
				std::make_pair(
					connectionId,
					std::move(function)
				)
			);

			return connectionId;
		}

		template < class T >
		ConnectionId connect(T* obj, RetVal(T::* mem_fn_ptr)(Args...))
		{
			return connect([=](Args... args) -> RetVal { return (obj->*mem_fn_ptr)(args...); });
		}

		ConnectionId connect(ConnectionTracker& connectionTracker, std::function<RetVal(Args ...)> function)
		{
			const auto connectionId = connect(std::forward(function));
			connectionTracker.trackConnection(ConnectionReference(*this, connectionId));
			return connectionId;
		}

		template < class T >
		ConnectionId connect(ConnectionTracker& connectionTracker, T* obj, RetVal(T::* mem_fn_ptr)(Args...))
		{
			const auto connectionId = connect(obj, mem_fn_ptr);
			connectionTracker.trackConnection(ConnectionReference(*this, connectionId));
			return connectionId;
		}

		void disconnect(const ConnectionId& connectionToRemove)
		{
			connectedElements.erase(
				std::find_if(
					connectedElements.begin(),
					connectedElements.end(),
					[&](auto& el) { return (el.first == connectionToRemove); }));
		}

		void disconnect(const std::vector<ConnectionId>& connectionsToRemove)
		{
			connectedElements.erase(
				std::remove_if( connectedElements.begin(),
								connectedElements.end(),
					[&](auto& el) { return std::any_of(connectionsToRemove.begin(), connectionsToRemove.end(), [&](const auto& conToRemove) {return el.first == conToRemove; }); }),
				connectedElements.end()
			);
		}

		bool isConnectionIdValid(const ConnectionId& connectionId)const
		{
			return (std::find_if(connectedElements.begin(), connectedElements.end(), [&](auto& el) { return (el.first == connectionId); }) != connectedElements.end());
		}

		void clear()
		{
			connectedElements.clear();
		}

		template< class ReturnValueHandler >
		void call(ReturnValueHandler returnValueHandler, Args ... args)const
		{
			// copy elements to enable adding/removing elements in recursive calls
			std::vector<std::function<RetVal(Args ...)>> toExecute;

			for (const auto& el : connectedElements)
				toExecute.push_back(el.second);

			for (const auto& el : toExecute)
				returnValueHandler(el(std::forward<Args>(args)...));
		}

		void call(Args ... args)const
		{
			// copy elements to enable adding/removing elements in recursive calls
			std::vector<std::function<RetVal(Args ...)>> toExecute;

			for (const auto& el : connectedElements)
				toExecute.push_back(el.second);

			for (const auto& el : toExecute)
				el(std::forward<Args>(args)...);
		}

		void operator() (Args ... args)
		{
			call( std::forward<Args>(args)... );
		}
	};


	using Connections = ConnectionTracker;
}
