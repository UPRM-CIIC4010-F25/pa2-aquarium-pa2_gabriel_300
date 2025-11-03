#include "Core.h"


// Creature Inherited Base Behavior
void Creature::setBounds(int w, int h) { m_width = w; m_height = h; }
void Creature::normalize() {
    float length = std::sqrt(m_dx * m_dx + m_dy * m_dy);
    if (length != 0) {
        m_dx /= length;
        m_dy /= length;
    }
}

void Creature::bounce() {
    // should implement boundary controls here
    //go side to side
    if (m_x < 0){
        m_x = 0;
        m_dx = std::abs(m_dx);
    } else if (m_x > m_width){
        m_x = m_width;
        m_dx = -std::abs(m_dx);
    }

// go up and down
    if (m_y < 0){
        m_y = 0;
        m_dy = 1;
    } else if (m_y > m_height){
        m_y = m_height;
        m_dy = -std::abs(m_dy);
    }

}


void GameEvent::print() const {
        
 if (type == GameEventType::NONE) {
        ofLogVerbose() << "Event: NONE";
    }
    else if (type == GameEventType::COLLISION) {
        if (creatureA && creatureB) {
            ofLogVerbose() << "Event: COLLISION at A("
                           << creatureA->getX() << ", " << creatureA->getY()
                           << ") vs B(" << creatureB->getX() << ", " << creatureB->getY() << ")";
        } else {
            ofLogVerbose() << "Event: COLLISION";
        }
    }
    else if (type == GameEventType::CREATURE_ADDED) {
        if (creatureA) {
            ofLogVerbose() << "Event: CREATURE_ADDED at ("
                           << creatureA->getX() << ", " << creatureA->getY() << ")";
        } else {
            ofLogVerbose() << "Event: CREATURE_ADDED";
        }
    }
    else if (type == GameEventType::CREATURE_REMOVED) {
        if (creatureA) {
            ofLogVerbose() << "Event: CREATURE_REMOVED at ("
                           << creatureA->getX() << ", " << creatureA->getY() << ")";
        } else {
            ofLogVerbose() << "Event: CREATURE_REMOVED";
        }
    }
    else if (type == GameEventType::GAME_OVER) {
        ofLogVerbose() << "Event: GAME_OVER";
    }
    else if (type == GameEventType::GAME_EXIT) {
        ofLogVerbose() << "Event: GAME_EXIT";
    }
    else if (type == GameEventType::NEW_LEVEL) {
        ofLogVerbose() << "Event: NEW_LEVEL";
    }
    else {
        ofLogVerbose() << "Event: UNKNOWN";
    }
    
};

// collision detection between two creatures
bool checkCollision(std::shared_ptr<Creature> a, std::shared_ptr<Creature> b) {
    
    if(!a || !b) return false;

    float dx = (a->getX() - b->getX());
    float dy = (a->getY() - b->getY());
    float r = a->getCollisionRadius() + b->getCollisionRadius();
    
    return (dx*dx + dy*dy) <= (r*r);

};


string GameSceneKindToString(GameSceneKind t){
    switch(t)
    {
        case GameSceneKind::GAME_INTRO: return "GAME_INTRO";
        case GameSceneKind::AQUARIUM_GAME: return "AQUARIUM_GAME";
        case GameSceneKind::GAME_OVER: return "GAME_OVER";
    };
};

std::shared_ptr<GameScene> GameSceneManager::GetScene(string name){
    if(!this->HasScenes()){return nullptr;}
    for(std::shared_ptr<GameScene> scene : this->m_scenes){
        if(scene->GetName() == name){
            return scene;
        }
    }
    return nullptr;
}

void GameSceneManager::Transition(string name){
    if(!this->HasScenes()){return;} // no need to do anything if nothing inside
    std::shared_ptr<GameScene> newScene = this->GetScene(name);
    if(newScene == nullptr){return;} // i dont have the scene so time to leave
    if(newScene->GetName() == this->m_active_scene->GetName()){return;} // another do nothing since active scene is already pulled
    this->m_active_scene = newScene; // now we keep it since this is a valid transition
    return;
}

void GameSceneManager::AddScene(std::shared_ptr<GameScene> newScene){
    if(this->GetScene(newScene->GetName()) != nullptr){
        return; // this scene already exist and shouldnt be added again
    }
    this->m_scenes.push_back(newScene);
    if(m_active_scene == nullptr){
        this->m_active_scene = newScene; // need to place in active scene as its the only one in existance right now
    }
    return;
}

std::shared_ptr<GameScene> GameSceneManager::GetActiveScene(){
    return this->m_active_scene;
}

string GameSceneManager::GetActiveSceneName(){
    if(this->m_active_scene == nullptr){return "";} // something to handle missing activate scenes
    return this->m_active_scene->GetName();
}

void GameSceneManager::UpdateActiveScene(){
    if(!this->HasScenes()){return;} // make sure we have a scene before we try to paint
    this->m_active_scene->Update();

}

void GameSceneManager::DrawActiveScene(){
    if(!this->HasScenes()){return;} // make sure we have something before Drawing it
    this->m_active_scene->Draw();
}


void GameIntroScene::Update(){

}

void GameIntroScene::Draw(){
    this->m_banner->draw(0,0);
}

void GameOverScene::Update(){

}

void GameOverScene::Draw(){
    ofBackgroundGradient(ofColor::red, ofColor::black);
    this->m_banner->draw(0,0);

}