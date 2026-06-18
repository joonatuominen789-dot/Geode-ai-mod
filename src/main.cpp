#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

// ====================================================================
// 1. ASETUKSET JA RAKENTEET
// ====================================================================
struct AIConfig {
    std::string chosenDifficulty = "Auto";
    int objectCount = 100;
    bool timeLimitHours = false;
};

// ====================================================================
// 2. PAUSE-VALIKON OHJAUSPANEELI
// ====================================================================
class AIPauseControlPanel : public CCMenu {
private:
    bool m_isAIActive = false;

public:
    static AIPauseControlPanel* create() {
        auto ret = new AIPauseControlPanel();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init() override {
        if (!CCMenu::init()) return false;

        this->setID("ai-pause-control-panel");
        
        auto layout = RowLayout::create();
        layout->setGap(10.f);
        this->setLayout(layout);

        this->setContentSize({ 150.f, 40.f });

        m_isAIActive = Mod::get()->getSavedValue<bool>("ai-enabled-state", false);

        auto label = CCLabelBMFont::create("PAUSE AI", "bigFont.fnt", 0.4f);
        this->addChild(label);

        std::string btnText = m_isAIActive ? "RESUME AI" : "PAUSE AI";
        auto btnSprite = ButtonSprite::create(btnText, "goldFont.fnt", "GJ_button_01.png", 0.6f);
        auto toggleBtn = CCMenuItemSpriteExtra::create(
            btnSprite,
            this,
            menu_selector(&AIPauseControlPanel::onPauseToggle)
        );
        toggleBtn->setID("ai-toggle-button");
        this->addChild(toggleBtn);

        if (m_isAIActive) {
            auto textLabel = typeinfo_cast<CCLabelBMFont*>(btnSprite->getChildren()->objectAtIndex(0));
            if (textLabel) textLabel->setColor({ 255, 100, 100 });
        }

        this->updateLayout();
        return true;
    }

    void onPauseToggle(CCObject* sender) {
        m_isAIActive = !m_isAIActive;
        Mod::get()->setSavedValue<bool>("ai-enabled-state", m_isAIActive);

        auto toggleBtn = typeinfo_cast<CCMenuItemSpriteExtra*>(sender);
        if (toggleBtn) {
            auto btnSprite = typeinfo_cast<ButtonSprite*>(toggleBtn->getChildren()->objectAtIndex(0));
            if (btnSprite) {
                auto textLabel = typeinfo_cast<CCLabelBMFont*>(btnSprite->getChildren()->objectAtIndex(0));
                if (textLabel) {
                    if (m_isAIActive) {
                        textLabel->setString("RESUME AI");
                        textLabel->setColor({ 255, 100, 100 });
                    } else {
                        textLabel->setString("PAUSE AI");
                        textLabel->setColor({ 255, 255, 255 });
                    }
                }
            }
        }
        
        log::info("AI Pause -tila muutettu: {}", m_isAIActive);
    }
};

// ====================================================================
// 3. AUTOMAATTISEN TALLENNUKSEN HALLINTA
// ====================================================================
class AIAUTOSAVE_MANAGER {
public:
    static void startAutosaveLoop() {
        bool isEnabled = Mod::get()->getSavedValue<bool>("ai-enabled-state", false);
        if (isEnabled) {
            log::info("Käynnistetään tekoälyn automaattitallennus...");
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
                schedule_selector(&AIAUTOSAVE_MANAGER::triggerAutosaveLoop),
                Mod::get(),
                60.0f,
                false
            );
        }
    }

    static void stopAutosaveLoop() {
        log::info("Pysäytetään tekoälyn automaattitallennus.");
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(
            schedule_selector(&AIAUTOSAVE_MANAGER::triggerAutosaveLoop),
            Mod::get()
        );
    }

    static void triggerAutosaveLoop(float dt) {
        if (auto editor = LevelEditorLayer::get()) {
            editor->quickSave();
            log::info("[AI-AUTOSAVE] Taso tallennettu automaattisesti taustalla.");
        }
    }
};
// ====================================================================
// 4. TEKOÄLYN ASETUSVALIKKO (POPUP)
// ====================================================================
class AISettingsPopup : public geode::Popup<AIConfig> {
protected:
    AIConfig m_config;
    CCMenuItemSpriteExtra* m_currentDifficultyBtn = nullptr;

    bool setup(AIConfig config) override {
        m_config = config;
        this->setTitle("AI Configuration", "bigFont.fnt", 0.7f);

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        auto bg = CCScale9Sprite::create("square02_001.png");
        bg->setContentSize({ 340.f, 220.f });
        bg->setColor({ 0, 0, 0 });
        bg->setOpacity(100);
        bg->setPosition({ winSize.width / 2, winSize.height / 2 });
        m_mainLayer->addChild(bg);

        auto difficultyMenu = CCMenu::create();
        difficultyMenu->setLayout(RowLayout::create()->setGap(8.f));
        difficultyMenu->setContentSize({ 300.f, 40.f });
        difficultyMenu->setPosition({ winSize.width / 2, winSize.height / 2 + 30.f });
        m_mainLayer->addChild(difficultyMenu);

        std::vector<std::string> difficulties = {"Auto", "Easy", "Normal", "Hard", "Demon"};
        for (const auto& diff : difficulties) {
            auto label = CCLabelBMFont::create(diff, "bigFont.fnt", 0.4f);
            auto btn = CCMenuItemSpriteExtra::create(
                label,
                this,
                menu_selector(&AISettingsPopup::onSelectDifficulty)
            );
            btn->setID(diff);
            difficultyMenu->addChild(btn);

            if (diff == m_config.chosenDifficulty) {
                m_currentDifficultyBtn = btn;
                btn->setScale(1.1f);
                label->setColor({ 100, 255, 100 });
            }
        }
        difficultyMenu->updateLayout();

        auto objectInput = CCTextInputNode::create(120.f, 30.f, "Objects", "bigFont.fnt");
        objectInput->setAllowedChars("0123456789");
        objectInput->setPosition({ winSize.width / 2 - 60.f, winSize.height / 2 - 20.f });
        m_mainLayer->addChild(objectInput);

        auto timeInput = CCTextInputNode::create(120.f, 30.f, "Time Limit", "bigFont.fnt");
        timeInput->setAllowedChars("0123456789");
        timeInput->setPosition({ winSize.width / 2 + 60.f, winSize.height / 2 - 20.f });
        m_mainLayer->addChild(timeInput);

        auto finishBtnSprite = ButtonSprite::create("FINISH", "goldFont.fnt", "GJ_button_01.png", 0.7f);
        auto finishBtn = CCMenuItemSpriteExtra::create(
            finishBtnSprite,
            this,
            menu_selector(&AISettingsPopup::onFinish)
        );
        m_buttonMenu->addChild(finishBtn);

        return true;
    }

    void onSelectDifficulty(CCObject* sender) {
        if (m_currentDifficultyBtn) {
            m_currentDifficultyBtn->setScale(1.0f);
            if (auto oldLabel = typeinfo_cast<CCLabelBMFont*>(m_currentDifficultyBtn->getChildren()->objectAtIndex(0))) {
                oldLabel->setColor({ 255, 255, 255 });
            }
        }

        m_currentDifficultyBtn = typeinfo_cast<CCMenuItemSpriteExtra*>(sender);
        if (m_currentDifficultyBtn) {
            m_currentDifficultyBtn->setScale(1.1f);
            m_config.chosenDifficulty = m_currentDifficultyBtn->getID();
            if (auto newLabel = typeinfo_cast<CCLabelBMFont*>(m_currentDifficultyBtn->getChildren()->objectAtIndex(0))) {
                newLabel->setColor({ 100, 255, 100 });
            }
        }
    }

    void onFinish(CCObject* sender) {
        if (auto objectInput = typeinfo_cast<CCTextInputNode*>(m_mainLayer->getChildByID("object-input"))) {
            if (!objectInput->getString().empty()) {
                m_config.objectCount = std::stoi(objectInput->getString());
            }
        }
        
        if (auto timeInput = typeinfo_cast<CCTextInputNode*>(m_mainLayer->getChildByID("time-input"))) {
            if (!timeInput->getString().empty()) {
                m_config.timeLimitHours = std::stoi(timeInput->getString()) > 0;
            }
        }

        Mod::get()->setSavedValue<std::string>("ai-difficulty", m_config.chosenDifficulty);
        Mod::get()->setSavedValue<int>("ai-object-count", m_config.objectCount);
        Mod::get()->setSavedValue<bool>("ai-time-limit", m_config.timeLimitHours);

        this->onClose(sender);
    }

public:
    static AISettingsPopup* create(AIConfig config) {
        auto ret = new AISettingsPopup();
        if (ret && ret->initAnchored(360.f, 240.f, config)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

// ====================================================================
// 5. SEURAAVAN SIVUN LISÄVALIKKO (OPTIOT)
// ====================================================================
class AIOptionsPopup : public geode::Popup<> {
protected:
    bool setup() override {
        this->setTitle("AI Advanced Options", "bigFont.fnt", 0.7f);
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto bg = CCScale9Sprite::create("square02_001.png");
        bg->setContentSize({ 300.f, 160.f });
        bg->setColor({ 0, 0, 0 });
        bg->setOpacity(120);
        bg->setPosition({ winSize.width / 2, winSize.height / 2 });
        m_mainLayer->addChild(bg);

        auto themeInput = CCTextInputNode::create(180.f, 30.f, "Custom AI Theme", "bigFont.fnt");
        themeInput->setPosition({ winSize.width / 2, winSize.height / 2 + 10.f });
        m_mainLayer->addChild(themeInput);

        auto closeBtnSprite = ButtonSprite::create("CLOSE", "goldFont.fnt", "GJ_button_06.png", 0.6f);
        auto closeBtn = CCMenuItemSpriteExtra::create(
            closeBtnSprite,
            this,
            menu_selector(&AIOptionsPopup::onClose)
        );
        m_buttonMenu->addChild(closeBtn);

        return true;
    }

public:
    static AIOptionsPopup* create() {
        auto ret = new AIOptionsPopup();
        if (ret && ret->initAnchored(320.f, 200.f)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

// ====================================================================
// 6. GEODE HOOKS (PELILAATAAJAN LIITÄNTÄÄ)
// ====================================================================
class $modify(MyEditorUI, EditorUI) {
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;

        log::info("Tekoälyn muokkaustyökalut liitetty EditorUI:hin!");

        auto pausePanel = AIPauseControlPanel::create();
        m_editGroupMenu->addChild(pausePanel);

        auto aiIconSprite = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
        auto aiMenuBtn = CCMenuItemSpriteExtra::create(
            aiIconSprite,
            this,
            menu_selector(&MyEditorUI::onAIButtonPressed)
        );
        aiMenuBtn->setID("ai-config-menu-button");
        m_editGroupMenu->addChild(aiMenuBtn);

        m_editGroupMenu->updateLayout();

        AIAUTOSAVE_MANAGER::startAutosaveLoop();
        return true;
    }

    void onAIButtonPressed(CCObject* sender) {
        AIConfig currentConfig;
        currentConfig.chosenDifficulty = Mod::get()->getSavedValue<std::string>("ai-difficulty", "Auto");
        currentConfig.objectCount = Mod::get()->getSavedValue<int>("ai-object-count", 100);
        currentConfig.timeLimitHours = Mod::get()->getSavedValue<bool>("ai-time-limit", false);

        if (auto popup = AISettingsPopup::create(currentConfig)) {
            popup->show();
        }
    }

    void onNextPage(CCObject* sender) {
        if (auto optionsPopup = AIOptionsPopup::create()) {
            optionsPopup->show();
        }
    }
};
