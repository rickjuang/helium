#if 1
/*************************************************************
 *	BaseRenderer
 *	by Rick Juang, 2011/12/19
 *	Base renderer class
 *************************************************************/

#include <utility/debug.h>

#include "base_renderer.h"

namespace Graphics
{

BaseRenderer* BaseRenderer::s_instance = NULL;

BaseRenderer::BaseRenderer(void)
{
	Dbg_Assert(s_instance == NULL);
	s_instance = this;
}

BaseRenderer::~BaseRenderer(void)
{
	s_instance = NULL;
}

void BaseRenderer::Init(float fps)
{
	m_renderTimer.SetFrequency(fps);
	m_renderTimer.TimeScale = 1.0;
	m_renderTimer.SetHandler(&DoRender);

	OnInit();
}

void BaseRenderer::Start(void)
{
	m_renderTimer.Reset();
	m_renderTimer.Start();

	OnStart();
}

void BaseRenderer::Render(void)
{
	OnRender();
}

void BaseRenderer::Shutdown(void)
{
	m_renderTimer.Stop();
	OnShutdown();
}

void BaseRenderer::DoRender(MSec delta_time)
{
	s_instance->Render();
}

}	// Namespace Graphics

#endif
