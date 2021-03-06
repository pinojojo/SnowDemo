#include "GLCore.h"
#include "ContainerLayer.h"

#include "ViewerLayerer.h"
#include "SliceViewer.h"

//#include "SliceVisualizer.h"

using namespace GLCore;

class MyGUI : public Application
{
public:
	MyGUI()
		:Application("Demo")
	{
		PushLayer(new ContainerLayer());
		
		PushLayer(new ViewerLayerer());

		PushLayer(new SliceViewer());

	}
};

int main()
{
	std::unique_ptr<MyGUI> app = std::make_unique<MyGUI>();
	app->Run();
}