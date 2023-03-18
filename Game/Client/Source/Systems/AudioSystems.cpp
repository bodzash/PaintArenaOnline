#include "Systems/AudioSystems.hpp"

void AudioPlayerSystem(entt::registry& Scene, std::map<string, Sound> Assets)
{
  auto View = Scene.view<Audio>();
  for (auto Entity : View)
  {
    auto& Aud = View.get<Audio>(Entity);

    if (Aud.bIsPlaying)
    {
      int Rnd = RandomInt(2);
      PlaySound(Assets[(Rnd == 1) ? Aud.Asset1 : Aud.Asset2]);
      Aud.bIsPlaying = false;
      if (Aud.bOneTime) Scene.remove<Audio>(Entity);
    }
  }
}