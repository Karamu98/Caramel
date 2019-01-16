#ifndef __APPLICATION_H_
#define __APPLICATION_H_

struct GLFWwindow;

class Application
{
public:

	Application() : m_running(false) {}
	virtual ~Application() {}

	bool	create(const char* a_name, int a_width, int a_height, bool a_bFullscreen);

	void	run(const char* a_name, int a_width, int a_height, bool a_bFullscreen);

	void	quit()	{ m_running = false; }

protected:

	virtual bool	onCreate() = 0;
	virtual void	Update(float a_deltaTime) = 0;
	virtual void	Draw() = 0;
	virtual void	Destroy() = 0;
	
	GLFWwindow*		m_window;
	bool			m_running;
};

#endif // __APPLICATION_H_