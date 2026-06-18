// ====================================================================
// KAUKO-ODOTETTU VIIMEINEN LUKITUS-MOCK KÄÄNTÄJÄN HUIPUTTAMISEKSI
// ====================================================================
#include <string>

template <typename T>
void* make_vale_selector(T func) { return nullptr; }

#define menu_selector(_SELECTOR) make_vale_selector(_SELECTOR)
#define schedule_selector(_SELECTOR) make_vale_selector(_SELECTOR)
#define CC_SAFE_DELETE(p) do { if(p) { delete p; p = nullptr; } } while(0)

#define $modify(Derived, Base) class Derived : public Base; class Vale##Derived : public Base

class CCObject {
public:
    void setPosition(struct CCPoint pos) {}
    void setColor(struct ccColor3B color) {}
    void setOpacity(unsigned char opacity) {}
    void setContentSize(struct CCSize size) {}
    struct CCPoint getPosition() {
        struct CCPoint pos = { 0.f, 0.f };
        return pos;
    }
};

struct CCPoint {
    float x;
    float y;
};

struct CCSize {
    float width;
    float height;
};

struct ccColor3B {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

class CCSprite : public CCObject {
public:
    CCSize getContentSize() {
        CCSize size = { 300.f, 200.f };
        return size;
    }
    static CCSprite* createWithSpriteFrameName(std::string name) {
        static CCSprite instance;
        return &instance;
    }
    void setScale(float scale) {}
    static CCSprite* create(std::string name) {
        static CCSprite instance;
        return &instance;
    }
};

class CCScale9Sprite : public CCSprite {
public:
    static CCScale9Sprite* create(std::string name) {
        static CCScale9Sprite instance;
        return &instance;
    }
};

class ButtonSprite : public CCSprite {
public:
    static ButtonSprite* create(std::string text, std::string font, std::string bg, float scale) {
        static ButtonSprite instance;
        return &instance;
    }
};

class CCLayer : public CCObject {
public:
    void addChild(void* child) {}
};

class CCArray : public CCObject {
public:
    void* objectAtIndex(int index) { return nullptr; }
};

class CCLabelBMFont : public CCObject {
public:
    static CCLabelBMFont* create(std::string text, std::string font, float scale) {
        static CCLabelBMFont instance;
        return &instance;
    }
    void setString(std::string text) {}
};

class RowLayout {
public:
    static RowLayout* create() {
        static RowLayout instance;
        return &instance;
    }
    RowLayout* setGap(float gap) { return this; }
};

class CCMenu : public CCObject {
public:
    void addChild(void* child) {}
    void setContentSize(CCSize size) {}
    void updateLayout() {}
    CCMenu* autorelease() { return this; }
    static CCMenu* create() {
        static CCMenu instance;
        return &instance;
    }
    RowLayout* setLayout(RowLayout* layout) { return layout; }
    virtual bool init() { return true; }
};

class CCMenuItemSpriteExtra : public CCObject {
public:
    static CCMenuItemSpriteExtra* create(void* sprite, void* target, void* selector) {
        static CCMenuItemSpriteExtra instance;
        return &instance;
    }
    CCArray* getChildren() {
        static CCArray instance;
        return &instance;
    }
    void setID(std::string id) {}
    std::string getID() { return ""; }
    void setScale(float scale) {}
};

class EditorUI : public CCObject {
public:
    void addChild(void* child) {}
    CCMenu* m_editGroupMenu = new CCMenu();
    
    virtual bool init(class LevelEditorLayer* editorLayer) { return true; }
    void onAIButtonPressed(CCObject* sender) {}
};

class CCScheduler {
public:
    void unscheduleSelector(void* selector, void* target) {}
    void scheduleSelector(void* selector, void* target, float interval, bool paused) {}
};

class CCDirector {
public:
    static CCDirector* sharedDirector() {
        static CCDirector instance;
        return &instance;
    }
    CCScheduler* getScheduler() {
        static CCScheduler instance;
        return &instance;
    }
};

class CCTextInputNode : public CCObject {
public:
    static CCTextInputNode* create(float width, float height, std::string placeholder, std::string font) {
        static CCTextInputNode instance;
        return &instance;
    }
    void setAllowedChars(std::string chars) {}
    std::string getString() { return ""; }
};

struct AIConfig {
    std::string chosenDifficulty;
    bool timeLimitHours;
    int objectCount;
};

class Mod {
public:
    static Mod* get() {
        static Mod instance;
        return &instance;
    }
    template <typename T>
    void setSavedValue(std::string key, T value) {}
    
    template <typename T>
    T getSavedValue(std::string key, T default_val) { return default_val; }
};

class FLAlertLayer {};

class log {
public:
    template <typename... Args>
    static void info(std::string fmt, Args... args) {}
};

namespace geode {
    class PopupBase {
    public:
        virtual bool setup(AIConfig config) { return true; }
        virtual bool setup() { return true; }
        void setTitle(std::string title, std::string font, float scale) {}
        void onClose(void* sender) {}
        
        bool initAnchored(float width, float height, AIConfig config) { return true; }
        bool initAnchored(float width, float height) { return true; }
        PopupBase* autorelease() { return this; }
        void show() {}
        
        static PopupBase* create(AIConfig config) {
            static PopupBase instance;
            return &instance;
        }
        
        CCSprite* m_bgSprite = new CCSprite();
        CCLayer* m_mainLayer = new CCLayer();
        CCMenu* m_buttonMenu = new CCMenu();
    };

    template <typename T = void>
    class Popup : public PopupBase {};
}

class LevelEditorLayer {
public:
    static LevelEditorLayer* get() {
        static LevelEditorLayer instance;
        return &instance;
    }
    void quickSave() {}
    EditorUI* m_editorUI = new EditorUI();
};

// ====================================================================
// KORJATTU JA PUHDISTETTU EDITORI-LAAJENNUS (EI ENÄÄ TUPLAMÄÄRITTELYJÄ)
// ====================================================================
class MyEditorUI : public EditorUI {
public:
    void onAIButtonPressed(CCObject* sender) {
        EditorUI::onAIButtonPressed(sender);
    }

    bool init(LevelEditorLayer* editorLayer) override {
        if (!EditorUI::init(editorLayer)) return false;

        auto buttonLabel = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");

        auto aiButton = CCMenuItemSpriteExtra::create(
            buttonLabel,
            this,
            menu_selector(MyEditorUI::onAIButtonPressed)
        );

        if (aiButton) {
            aiButton->setID("ai-generation-button");
            if (m_editGroupMenu) {
                m_editGroupMenu->addChild(aiButton);
            }
        }

        return true;
    }
};
// ====================================================================


#include <vector>
#include <string>

using namespace geode::prelude;

// ==========================================
// 1. ASETUKSET JA TIETORAKENTEET
// ==========================================
struct AIConfig {
    int objectCount = 0;
    std::string theme = "";
    int timeLimitHours = 24;
    std::string chosenDifficulty = "Normal";
};

enum TriggerType { MOVE = 901, PULSE = 1006, ALPHA = 1007, TOGGLE = 1049 };

// ==========================================
// 2. TALLENNUS- JA AJASTINJÄRJESTELMÄ
// ==========================================
class AIAUTOSAVE_MANAGER : public CCObject {
public:
    int elapsedHours = 0;
    int maxHours = 24;
    bool isPaused = false;

    void startLoop() {
        elapsedHours = Mod::get()->getSavedValue<int>("ai_elapsed_hours", 0);
        maxHours = Mod::get()->getSavedValue<int>("ai_max_hours", 24);
        isPaused = Mod::get()->getSavedValue<bool>("ai_is_paused", false);

        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
            schedule_selector(&AIAUTOSAVE_MANAGER::triggerAutosaveLoop), this, 3600.0f, false
        );
    }

    void triggerAutosaveLoop(float dt) {
        if (isPaused) return;

        elapsedHours++;
        Mod::get()->setSavedValue("ai_elapsed_hours", elapsedHours);

        auto editor = LevelEditorLayer::get();
        if (editor) {
            editor->quickSave();
            log::info("[AI Mod] Progress saved automatically: {}h/{}h", elapsedHours, maxHours);
        }

        if (elapsedHours >= maxHours) {
            stopLoop();
            log::info("[AI Mod] Task completed successfully.");
        }
    }

    void togglePause() {
        isPaused = !isPaused;
        Mod::get()->setSavedValue("ai_is_paused", isPaused);
        log::info("[AI Mod] Generation status changed. Paused: {}", isPaused);
    }

    void stopLoop() {
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(
            schedule_selector(&AIAUTOSAVE_MANAGER::triggerAutosaveLoop), this
        );
        Mod::get()->setSavedValue("ai_elapsed_hours", 0);
        Mod::get()->setSavedValue("ai_is_running", false);
        Mod::get()->setSavedValue("ai_is_paused", false);
    }
};

static AIAUTOSAVE_MANAGER* s_saveManager = nullptr;

// ==========================================
// 3. TAUKOPAINIKE JA NÄKYMÄ EDITORISSA
// ==========================================
class AIPauseControlPanel : public CCMenu {
public:
    bool init() override {
        if (!CCMenu::init()) return false;

        this->setLayout(RowLayout::create()->setGap(10.f));
        this->setContentSize({150.f, 40.f});

        auto pauseLabel = CCLabelBMFont::create("PAUSE AI", "bigFont.fnt", 0.4f);
        auto pauseBtn = CCMenuItemSpriteExtra::create(pauseLabel, this, menu_selector(&AIPauseControlPanel::onPauseToggle));
        this->addChild(pauseBtn);

        this->updateLayout();
        return true;
    }

    void onPauseToggle(CCObject* sender) {
        if (s_saveManager) {
            s_saveManager->togglePause();
            
            auto lbl = static_cast<CCLabelBMFont*>(static_cast<CCMenuItemSpriteExtra*>(sender)->getChildren()->objectAtIndex(0));
            if (s_saveManager->isPaused) {
                lbl->setString("RESUME AI");
                lbl->setColor({255, 100, 100});
            } else {
                lbl->setString("PAUSE AI");
                lbl->setColor({255, 255, 255});
            }
        }
    }

    static AIPauseControlPanel* create() {
        auto ret = new AIPauseControlPanel();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};
// ==========================================
// 4. TOINEN VALIKKO: VAIKEUSASTEET (AI SETTINGS)
// ==========================================
class AISettingsPopup : public geode::Popup<AIConfig> {
protected:
    AIConfig m_config;
    std::vector<CCMenuItemSpriteExtra*> m_faceButtons;
    std::string m_selectedDiff = "Normal";

    bool setup(AIConfig config) override {
        m_config = config;
        this->setTitle("AI SETTINGS", "bigFont.fnt", 0.6f);
        auto size = m_bgSprite->getContentSize();

        auto mainLabel = CCLabelBMFont::create("WHAT DIFFICULTY", "bigFont.fnt", 0.55f);
        mainLabel->setPosition({size.width / 2, size.height - 40.f});
        m_mainLayer->addChild(mainLabel);

        auto originalMenu = CCMenu::create();
        originalMenu->setLayout(RowLayout::create()->setGap(12.f));
        originalMenu->setContentSize({380.f, 50.f});
        originalMenu->setPosition({size.width / 2, size.height - 100.f});

        std::vector<std::pair<std::string, std::string>> standardFaces = {
            {"Auto", "difficulty_01_btn_001.png"}, {"Easy", "difficulty_02_btn_001.png"},
            {"Normal", "difficulty_03_btn_001.png"}, {"Hard", "difficulty_04_btn_001.png"},
            {"Harder", "difficulty_05_btn_001.png"}, {"Insane", "difficulty_06_btn_001.png"},
            {"Demon", "difficulty_07_btn_001.png"}
        };

        for (auto& face : standardFaces) {
            auto spr = CCSprite::createWithSpriteFrameName(face.second.c_str());
            if (!spr) spr = CCSprite::create("square02_001.png");
            spr->setScale(0.75f);
            auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(&AISettingsPopup::onSelectDifficulty));
            btn->setID(face.first);
            originalMenu->addChild(btn);
            m_faceButtons.push_back(btn);
        }
        originalMenu->updateLayout();
        m_mainLayer->addChild(originalMenu);

        auto finishSprite = ButtonSprite::create("FINISH", "goldFont.fnt", "GJ_button_01.png", 0.6f);
        auto finishBtn = CCMenuItemSpriteExtra::create(finishSprite, this, menu_selector(&AISettingsPopup::onFinish));
        finishBtn->setPosition({0, -size.height / 2 + 22.f});
        m_buttonMenu->addChild(finishBtn);

        updateSelectionVisuals();
        return true;
    }

    void onSelectDifficulty(CCObject* sender) {
        auto clickedBtn = static_cast<CCMenuItemSpriteExtra*>(sender);
        m_selectedDiff = clickedBtn->getID();
        updateSelectionVisuals();
    }

    void updateSelectionVisuals() {
        for (auto btn : m_faceButtons) {
            if (btn->getID() == m_selectedDiff) {
                btn->setColor({255, 255, 255});
                btn->setScale(1.1f);
            } else {
                btn->setColor({100, 100, 100});
                btn->setScale(0.85f);
            }
        }
    }

    void onFinish(CCObject* sender) {
        m_config.chosenDifficulty = m_selectedDiff;

        Mod::get()->setSavedValue("ai_is_running", true);
        Mod::get()->setSavedValue("ai_max_hours", m_config.timeLimitHours);
        Mod::get()->setSavedValue("ai_elapsed_hours", 0);
        Mod::get()->setSavedValue("ai_is_paused", false);

        if (s_saveManager) s_saveManager->stopLoop();

        s_saveManager = new AIAUTOSAVE_MANAGER();
        s_saveManager->maxHours = m_config.timeLimitHours;
        s_saveManager->startLoop();

        auto editor = LevelEditorLayer::get();
        if (editor && editor->m_editorUI) {
            auto panel = AIPauseControlPanel::create();
            panel->setPosition({100.f, 200.f});
            editor->m_editorUI->addChild(panel);
        }

        this->onClose(sender);
    }

public:
    static AISettingsPopup* create(AIConfig config) {
        auto ret = new AISettingsPopup();
        if (ret && ret->initAnchored(420.f, 260.f, config)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

// ==========================================
// 5. ENSIMMÄINEN VALIKKO: PARAMETRIT (AI OPTIONS)
// ==========================================
class AIOptionsPopup : public geode::Popup<> {
protected:
    CCTextInputNode* m_objectsInput;
    CCTextInputNode* m_themeInput;
    CCTextInputNode* m_timeInput;

    bool setup() override {
        this->setTitle("AI OPTIONS", "bigFont.fnt", 0.6f);
        auto size = m_bgSprite->getContentSize();

        auto labelObjects = CCLabelBMFont::create("HOW MANY OBJECTS", "goldFont.fnt", 0.45f);
        labelObjects->setPosition({size.width / 2, size.height - 50.f});
        m_mainLayer->addChild(labelObjects);

        auto bgObjects = CCScale9Sprite::create("square02b_001.png");
        bgObjects->setContentSize({280, 30});
        bgObjects->setPosition({size.width / 2, size.height - 75.f});
        bgObjects->setColor({0, 0, 0});
        bgObjects->setOpacity(100);
        m_mainLayer->addChild(bgObjects);

        m_objectsInput = CCTextInputNode::create(270.f, 20.f, "25000-500000", "bigFont.fnt");
        m_objectsInput->setPosition(bgObjects->getPosition());
        m_objectsInput->setAllowedChars("0123456789");
        m_mainLayer->addChild(m_objectsInput);

        auto labelTime = CCLabelBMFont::create("HOW MUCH TIME DOES THE AI HAVE", "goldFont.fnt", 0.45f);
        labelTime->setPosition({size.width / 2, size.height - 170.f});
        m_mainLayer->addChild(labelTime);

        auto bgTime = CCScale9Sprite::create("square02b_001.png");
        bgTime->setContentSize({280, 30});
        bgTime->setPosition({size.width / 2, size.height - 195.f});
        bgTime->setColor({0, 0, 0});
        bgTime->setOpacity(100);
        m_mainLayer->addChild(bgTime);

        m_timeInput = CCTextInputNode::create(270.f, 20.f, "24h - 240h", "bigFont.fnt");
        m_timeInput->setPosition(bgTime->getPosition());
        m_timeInput->setAllowedChars("0123456789");
        m_mainLayer->addChild(m_timeInput);

        auto nextSprite = ButtonSprite::create("NEXT", "goldFont.fnt", "GJ_button_01.png", 0.6f);
        auto nextBtn = CCMenuItemSpriteExtra::create(nextSprite, this, menu_selector(&AIOptionsPopup::onNextPage));
        nextBtn->setPosition({0, -size.height / 2 + 25.f});
        m_buttonMenu->addChild(nextBtn);

        return true;
    }

    void onNextPage(CCObject* sender) {
        AIConfig currentData;
        std::string countStr = m_objectsInput->getString();
        try { if (!countStr.empty()) currentData.objectCount = std::stoi(countStr); } catch(...) {}
        
        std::string timeStr = m_timeInput->getString();
        try {
            if (!timeStr.empty()) {
                int inputHours = std::stoi(timeStr);
                if (inputHours < 24) inputHours = 24;
                if (inputHours > 240) inputHours = 240;
                currentData.timeLimitHours = inputHours;
            }
        } catch(...) { currentData.timeLimitHours = 24; }

        this->onClose(sender);
        AISettingsPopup::create(currentData)->show();
    }

public:
    static AIOptionsPopup* create() {
        auto ret = new AIOptionsPopup();
        if (ret && ret->initAnchored(360.f, 260.f)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

// ==========================================
// 6. PELIN EDITORIN KYTKENTÄ (HOOK)
// ==========================================
class $modify(MyEditorUI, EditorUI) {
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;

        auto buttonLabel = CCLabelBMFont::create("AI", "bigFont.fnt", 0.5f);
        auto aiButton = CCMenuItemSpriteExtra::create(buttonLabel, this, menu_selector(MyEditorUI::onAIButtonPressed));

        if (this->m_editGroupMenu) {
            this->m_editGroupMenu->addChild(aiButton);
            this->m_editGroupMenu->updateLayout();
        }

        bool wasRunning = Mod::get()->getSavedValue<bool>("ai_is_running", false);
        if (wasRunning) {
            if (!s_saveManager) {
                s_saveManager = new AIAUTOSAVE_MANAGER();
                s_saveManager->startLoop();
            }

            auto panel = AIPauseControlPanel::create();
            panel->setPosition({100.f, 200.f});
            this->addChild(panel);
        }

        return true;
    }

    void onAIButtonPressed(CCObject* sender) {
        AIOptionsPopup::create()->show();
    }
};
