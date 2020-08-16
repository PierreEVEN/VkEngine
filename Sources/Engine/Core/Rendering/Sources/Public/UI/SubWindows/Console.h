#pragma once

#include "SubWindow.h"

namespace Rendering {
	class Console : public SubWindow 
	{
	public:

		Console();



	protected:

		virtual void DrawContent(const size_t& imageIndex);

	private:


		struct ConsoleMessage {
			String text;
			ConsoleColor textColor;
		};

		char ConsoleInputBuffer[256];
		bool bScrollToEnd = true;
		void OnSendMessage(const String& message);

		std::vector<ConsoleMessage> messages;

		ConsoleMessage currentMessage;

		~Console();
	};
}