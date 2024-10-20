#include <iostream>

#include <spdlog/spdlog.h>
#include <window/Window.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <utils/Log.h>

int main(int argc, char* argv[]) {
	auto window = std::make_unique<Window>(500, 500, "test");

	while (!window->is_shouldClose()) {
		window->e_update();

		if (Input::IsMouseButtonPressed(Mouse::Codes::MOUSE_BUTTON_LEFT)) {
			glClearColor(0.6f, 0.62f, 0.65f, 1);
		}

		if (Input::IsMouseButtonPressed(Mouse::Codes::MOUSE_BUTTON_RIGHT)) {
			glClearColor(0.8f, 0.4f, 0.2f, 1);
		}

		glClear(GL_COLOR_BUFFER_BIT);
		window->w_update();
	}

	return 0;
}
