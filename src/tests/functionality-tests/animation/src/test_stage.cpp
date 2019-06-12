#include "test_stage.h"
#include "registry.h"

// Delete these later
#include "components/sprite_component.h"
#include "components/sprite_animation_component.h"
#include "components/time_component.h"
#include "components/position_component.h"

// End of delete

using namespace Halley;

#include <iostream>


void TestStage::init()
{
	std::cout << "0 Init" << std::endl;
	world = createWorld("sample_test_world", createSystem);
	statsView = std::make_unique<WorldStatsView>(*getAPI().core);
	statsView->setWorld(world.get());
	
	/*auto anim = getResources().get<Animation>("ella");
	world->createEntity()
		.addComponent(PositionComponent(Vector2f(10, 10)))
		.addComponent(SpriteComponent(Sprite(), 0))
		.addComponent(TimeComponent(0))
		.addComponent(SpriteAnimationComponent(AnimationPlayer(anim, "idle")));*/
}

void TestStage::onFixedUpdate(Time time)
{
	std::cout << "1 Fixed" << std::endl;
	world->step(TimeLine::FixedUpdate, time);
}

void TestStage::onVariableUpdate(Time time)
{
	std::cout << "2 Variable" << std::endl;
	world->step(TimeLine::VariableUpdate, time);

	auto key = getInputAPI().getKeyboard();
	if (key->isButtonDown(Keys::Esc)) {
		getCoreAPI().quit();
	}
}

void TestStage::onRender(RenderContext& context) const
{
	std::cout << "3 Render" << std::endl;
	world->render(context);
	statsView->draw(context);
}
