#pragma once

class GLFWwindow;

class MinecraftWindow 
{
public:
	explicit MinecraftWindow(const char* title, int w, int h);
	~MinecraftWindow() noexcept;

private:
	GLFWwindow* window;

public:
	bool IsClose();
	void SwapBuffer();
	void PoolEvent();
	void Terminate();

	inline GLFWwindow* GetWindow() const { return window; }
};