#include "halley_editor.h"
#include "editor_root_stage.h"
#include "halley/tools/project/project.h"
#include "preferences.h"
#include "halley/core/game/environment.h"
#include "halley/tools/file/filesystem.h"
#include "halley/tools/project/project_loader.h"

using namespace Halley;

void initOpenGLPlugin(IPluginRegistry &registry);
void initSDLSystemPlugin(IPluginRegistry &registry);
void initSDLAudioPlugin(IPluginRegistry &registry);
void initSDLInputPlugin(IPluginRegistry &registry);
void initAsioPlugin(IPluginRegistry &registry);
void initDX11Plugin(IPluginRegistry &registry);

HalleyEditor::HalleyEditor()
{
}

HalleyEditor::~HalleyEditor()
{
}

int HalleyEditor::initPlugins(IPluginRegistry &registry)
{
	initSDLSystemPlugin(registry);
	initAsioPlugin(registry);
	initSDLAudioPlugin(registry);
	initSDLInputPlugin(registry);

#ifdef _WIN32
	initDX11Plugin(registry);
#else
	initOpenGLPlugin(registry);
#endif
	
	return HalleyAPIFlags::Video | HalleyAPIFlags::Audio | HalleyAPIFlags::Input | HalleyAPIFlags::Network;
}

void HalleyEditor::initResourceLocator(const Path& gamePath, const Path& assetsPath, const Path& unpackedAssetsPath, ResourceLocator& locator)
{
	locator.addFileSystem(unpackedAssetsPath);
}

String HalleyEditor::getName() const
{
	return "Halley Editor";
}

String HalleyEditor::getDataPath() const
{
	return "halley/editor";
}

bool HalleyEditor::isDevMode() const
{
	return true;
}

bool HalleyEditor::shouldCreateSeparateConsole() const
{
#ifdef _DEBUG
	return isDevMode();
#else
	return false;
#endif
}

Preferences& HalleyEditor::getPreferences()
{
	return *preferences;
}

void HalleyEditor::init(const Environment& environment, const Vector<String>& args)
{
	rootPath = environment.getProgramPath().parentPath();

	parseArguments(args);
}

void HalleyEditor::parseArguments(const std::vector<String>& args)
{
	platform = "pc";
	gotProjectPath = false;

	for (auto& arg : args) {
		if (arg.startsWith("--")) {
			if (arg.startsWith("--platform=")) {
				platform = arg.mid(String("--platform=").length());
			} else {
				std::cout << "Unknown argument \"" << arg << "\".\n";
			}
		} else {
			if (!gotProjectPath) {
				projectPath = arg.cppStr();
				gotProjectPath = true;
			} else {
				std::cout << "Unknown argument \"" << arg << "\".\n";
			}
		}
	}
}

std::unique_ptr<Stage> HalleyEditor::startGame(const HalleyAPI* api)
{
	preferences = std::make_unique<Preferences>(*api->system);

	projectLoader = std::make_unique<ProjectLoader>(api->core->getStatics(), rootPath);
	projectLoader->setPlatform(platform);

	if (gotProjectPath) {
		loadProject(Path(projectPath));
	}

	api->video->setWindow(preferences->getWindowDefinition(), true);
	return std::make_unique<EditorRootStage>(*this);
}

Project& HalleyEditor::loadProject(Path path)
{
	project = projectLoader->loadProject(path);

	if (!project) {
		throw Exception("Unable to load project at " + path.string());
	}

	preferences->addRecent(path.string());
	preferences->saveToFile();
	
	return *project;
}

Project& HalleyEditor::createProject(Path path)
{
	project.reset();

	if (!project) {
		throw Exception("Unable to create project at " + path.string());
	}
	
	preferences->addRecent(path.string());
	preferences->saveToFile();
	
	return *project;
}

bool HalleyEditor::hasProjectLoaded() const
{
	return static_cast<bool>(project);
}

Project& HalleyEditor::getProject() const
{
	if (!project) {
		throw Exception("No project loaded.");
	}
	return *project;
}

HalleyGame(HalleyEditor);
