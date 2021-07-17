#pragma once

struct GLFWwindow;
class TEngine
{
	bool		mRun;
	uint16_t	mWidth, mHeight;
	std::string	mTitle;
	GLFWwindow* mWindow;
public:
	~TEngine() noexcept;
	TEngine(const TEngine& other) noexcept = delete;
	TEngine& operator=(const TEngine& other) noexcept = delete;

	void init() noexcept;
	void update() noexcept;
	void quit() noexcept;

	inline bool isRunning() const noexcept { return mRun; }
	inline GLFWwindow& getWindow() noexcept { return *mWindow; }
private:
	TEngine() noexcept;
public:
	static TEngine& get() noexcept;
};

static TEngine& Engine = TEngine::get();