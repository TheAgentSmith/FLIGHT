#pragma once

#include <iostream>
#include <memory>
#include <stack>
#include <glm/glm.hpp>

#include "Time.hpp"
#include "ThreadGuard.hpp"
#include "Reticle.hpp"

class Scene;

extern std::mutex g_updateMtx;

void DisplayShadowOverlay(const float amount);

using SceneStack = std::stack<std::unique_ptr<Scene>>;

class Scene {
public:
    virtual void UpdateLogic(const Time dt) = 0;
    virtual void UpdateState(SceneStack & state) = 0;
    virtual bool Display() = 0;
    virtual ~Scene() {}
};

class TitleScreen : public Scene {
public:
    void UpdateLogic(const Time dt) override;
    void UpdateState(SceneStack & state) override;
    bool Display() override;
};

class WorldLoader : public Scene {
    bool m_active;
    ThreadGuard m_terrainThread;
public:
    WorldLoader();
    void UpdateLogic(const Time dt) override;
    void UpdateState(SceneStack & state) override;
    bool Display() override;
    ~WorldLoader() {
	m_active = false;
    }
};

class World : public Scene {
    void DrawTerrain();
    void DrawSky();
    void DrawOverlays();
    void UpdatePerspProjUniforms();
    void UpdateOrthoProjUniforms();
    Reticle m_reticle; // <-- TODO: create UI class and move this there.
public:
    World();
    void UpdateLogic(const Time dt) override;
    void UpdateState(SceneStack & state) override;
    bool Display() override;
};

class WorldTransitionIn : public World {
    Time m_transitionTimer;
    static const Time TRANSITION_TIME = 700000;
public:
    void UpdateLogic(const Time dt) override;
    void UpdateState(SceneStack & state) override;
    bool Display() override;
};

class Menu : public Scene {
public:
    void UpdateLogic(const Time dt) override;
    void UpdateState(SceneStack & state) override;
    bool Display() override;
};

class MenuTransitionIn : public World {
    Time m_transitionTimer;
    static const Time TRANSITION_TIME = 400000;
public:
    MenuTransitionIn();
    void UpdateLogic(const Time dt) override;
    void UpdateState(SceneStack & state) override;
    bool Display() override;
};

class MenuTransitionOut : public World {
    Time m_transitionTimer;
    static const Time TRANSITION_TIME = 300000;
public:
    MenuTransitionOut();
    void UpdateLogic(const Time dt) override;
    void UpdateState(SceneStack & state) override;
    bool Display() override;
};
