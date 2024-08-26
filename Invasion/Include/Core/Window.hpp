#pragma once

#include "Core/Logger.hpp"
#include "Core/Settings.hpp"
#include "Math/Vector2.hpp"
#include "Util/Typedefs.hpp"

using namespace Invasion::Core;
using namespace Invasion::Math;
using namespace Invasion::Util;

namespace Invasion::Core
{
	class Window
	{

	public:

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		void Initialize(const String& title, const Vector2i& dimensions)
		{
			WNDCLASSEX windowClass = { 0 };

			windowClass.cbSize = sizeof(WNDCLASSEX);
			windowClass.style = CS_HREDRAW | CS_VREDRAW;
			windowClass.lpfnWndProc = Settings::GetInstance().Get<WNDPROC>("defaultWindowProcedure");
			windowClass.hInstance = GetModuleHandle(nullptr);
			windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
			windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			windowClass.lpszClassName = "Invasion_Engine";

			RegisterClassEx(&windowClass);

			handle = CreateWindowEx(0, "Invasion_Engine", title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, dimensions.x, dimensions.y, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);

			if (!handle)
				Logger_ThrowException("Failed to create window.", true);

			assert(handle != nullptr, "Failed to create window.");

			ShowWindow(handle, SW_SHOW);
			
			UpdateWindow(handle);
		}

		void Run()
		{
			MSG message = { 0 };
			bool isRunning = true;

			while (isRunning)
			{
				if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&message);
					DispatchMessage(&message);

					if (message.message == WM_QUIT)
						isRunning = false;
				}

				updateFunctions.ForEach([](Function<void()>& function) { function(); });
				renderFunctions.ForEach([](Function<void()>& function) { function(); });
			}
		}

		void AddUpdateFunction(const Function<void()>& function)
		{
			updateFunctions += function;
		}

		void AddRenderFunction(const Function<void()>& function)
		{
			renderFunctions += function;
		}

		HWND GetHandle() const
		{
			return handle;
		}

		void SetTitle(const String& title)
		{
			SetWindowText(handle, title);
		}

		String GetTitle() const
		{
			Size length = GetWindowTextLength(handle);

			String title(length, '\0');

			GetWindowText(handle, title, length + 1);

			return title;
		}

		void SetDimensions(const Vector2i& dimensions)
		{
			RECT rect = { 0, 0, dimensions.x, dimensions.y };
			AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

			SetWindowPos(handle, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER);
		}

		Vector2i GetClientDimensions() const
		{
			RECT rect;
			GetClientRect(handle, &rect);

			return Vector2i(rect.right - rect.left, rect.bottom - rect.top);
		}

		Vector2i GetDimensions() const
		{
			RECT rect;
			GetWindowRect(handle, &rect);

			return Vector2i(rect.right - rect.left, rect.bottom - rect.top);
		}

		void SetPosition(const Vector2i& position)
		{
			SetWindowPos(handle, nullptr, position.x, position.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}

		Vector2i GetPosition() const
		{
			RECT rect;
			GetWindowRect(handle, &rect);

			return Vector2i(rect.left, rect.top);
		}

		float GetAspectRatio() const
		{
			Vector2i dimensions = GetClientDimensions();

			return static_cast<float>(dimensions.x) / static_cast<float>(dimensions.y);
		}

		float GetDeltaTime()
		{
			TimePoint currentTime = SystemClock::now();

			Duration deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			return deltaTime.count();
		}

		void CleanUp()
		{
			UnregisterClass("Invasion_Engine", GetModuleHandle(nullptr));

			DestroyWindow(handle);

			handle = nullptr;
		}

		static Window& GetInstance()
		{
			static Window instance;
			return instance;
		}

	private:

		Window() = default;

		Vector<Function<void()>> updateFunctions;
		Vector<Function<void()>> renderFunctions;

		HWND handle = nullptr;

		TimePoint lastTime;
	};
}