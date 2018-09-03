#pragma once

class Scene;

typedef std::map<CString, Scene*> MScene;
typedef MScene::value_type MScenePair;
typedef MScene::iterator IterScene;

class SceneManager
{
private:
	SceneManager();
	~SceneManager();

public:
	static SceneManager* GetInstance();
	void Delete();

	void Awake();
	void Update();
	void Render();

	void AddScene(Scene* pScene);
	void RemoveScene(CString szName);
	void ChangeScene(CString szName);

	Scene* GetCurrentScene() { return m_pCurrentScene; }

private:
	static SceneManager* m_pInstance;

	Scene* m_pCurrentScene;
	std::map<CString, Scene*> m_mapScenes;

	bool m_bPause;

	GameObject* m_pUILoading;
};