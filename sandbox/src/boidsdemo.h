
#include <prevengine.h>

#include <random>
#include <vector>

namespace prev {

  static const pvfloat MAX_VEL = 1.0f;
  static const pvfloat MAX_VEL_SQ = MAX_VEL * MAX_VEL;

  static const pvfloat MAX_ACCEL = 4.0f;
  static const pvfloat MAX_ACCEL_SQ = MAX_ACCEL * MAX_ACCEL;

  static pvfloat s_ViewRadius = 0.2f;
  static pvfloat s_ViewRadiusSq = s_ViewRadius * s_ViewRadius;

  static const int MAX_BOIDS = 50;

  struct Boid {
    Vec2 Position     = Vec2(0.0f, 0.0f);
    Vec2 Scale        = Vec2(0.03f);
    Vec2 Velocity     = Vec2(1.0f, 1.0f);
    Vec2 Acceleration = Vec2(0.0f, 0.0f);

    void Update(pvfloat deltaTime) {

      if ( Vec2::LengthSqr(Acceleration) > MAX_ACCEL_SQ ) {
        Acceleration = Vec2::Normalize(Acceleration) * MAX_ACCEL;
      }

      Velocity += Acceleration * deltaTime;
      
      if ( Vec2::LengthSqr( Velocity ) > MAX_VEL_SQ ) {
        Velocity = Vec2::Normalize(Velocity) * MAX_VEL;
      }

      Position += Velocity * deltaTime;

      Acceleration = { 0.0f, 0.0f };
    }

    void Render() const {
      Vec2 a = Vec2(0.0f, 1.0f) * Scale;
      Vec2 b = Vec2(0.5f, -0.5f) * Scale;
      Vec2 c = Vec2(-0.5f, -0.5f) * Scale;

      pvfloat angle = 0.0f;

      // calculate angle
      {
        pvfloat vel_mag = Vec2::Length( Velocity );

        if ( vel_mag != 0.0f ) {
          angle = Velocity.x / vel_mag;
        }

        angle = std::acos( angle );
        angle = ToDegrees( angle );

        if ( Velocity.y < 0.0f )
          angle = 360.0f - angle;

        angle -= 90;
      }


      a = Vec3::RotateAxisZ(Vec3( a.x, a.y, 0.0f ), angle).xy();
      b = Vec3::RotateAxisZ(Vec3( b.x, b.y, 0.0f ), angle).xy();
      c = Vec3::RotateAxisZ(Vec3( c.x, c.y, 0.0f ), angle).xy();

      ImmGFX::Ref().Color(0.0f, 1.0f, 1.0f, 1.0f);
      ImmGFX::Ref().DrawTriangle(a + Position, b + Position, c + Position);
      ImmGFX::Ref().Color(0.0f);
    }
  };

  class BoidsDemo : public Layer {
  public:

    virtual void OnAttach() override {
      float width = Window::Ref().GetWidth();
      float height = Window::Ref().GetHeight();

      float aspect = width / height;

      std::mt19937 rng((unsigned int)Timer::GetTime().GetUS());
      std::uniform_real_distribution<pvfloat> gen(-0.8f, 0.8f);

      for (int i = 0; i < MAX_BOIDS; i++) {
        Boid b;
        b.Position.x = gen(rng);
        b.Position.y = gen(rng);
        m_Boids.push_back(b);
      }

    }

    virtual void OnUpdate(pvfloat deltaTime) override {
      float width  = Window::Ref().GetWidth();
      float height = Window::Ref().GetHeight();

      float aspect = width / height;

      size_t i = 0;
      for ( auto& b : m_Boids ) {
        if ( b.Position.x > aspect - 0.1f ) {
          b.Acceleration.x -= 5.0f;
        }
        if ( b.Position.x < -aspect + 0.1f ) {
          b.Acceleration.x += 5.0f;
        }
        if ( b.Position.y > 1.0f - 0.1f ) {
          b.Acceleration.y -= 5.0f;
        }
        if ( b.Position.y < -1.0f + 0.1f ) {
          b.Acceleration.y += 5.0f;
        }

        Steer(i);
        Align(i);
        b.Update( deltaTime );
        b.Render();

        i++;
      }

      DrawDebug(0);

    }

    void Steer(size_t index) {
      if (index >= m_Boids.size()) return;

      Vec2 steeringForce;
      float totalForce = 0.0f;

      for ( size_t i = 0; i < m_Boids.size(); i++ ) {
        if ( i == index ) continue;
        Vec2 dist = m_Boids[index].Position - m_Boids[i].Position;
        if ( Vec2::LengthSqr(dist) < s_ViewRadiusSq ) {
          pvfloat d = Vec2::Length(dist);
          dist /= d * d;
          steeringForce += dist;
          totalForce++;
        }
      }

      if ( totalForce > 0.0f ) {
        steeringForce /= totalForce;
        m_Boids[index].Acceleration += steeringForce;
      }
    }

    void Align(size_t index) {
      if (index >= m_Boids.size()) return;

      Vec2 alignForce;
      float totalForce = 0.0f;

      for ( size_t i = 0; i < m_Boids.size(); i++ ) {
        if ( i == index ) continue;
        Vec2 dist = m_Boids[index].Position - m_Boids[i].Position;
        if ( Vec2::LengthSqr(dist) < s_ViewRadiusSq ) {
          alignForce += m_Boids[i].Velocity;
          totalForce++;
        }
      }

      if ( totalForce > 0.0f ) {
        alignForce /= totalForce;
        m_Boids[index].Acceleration += alignForce * 2.0f;
      }

    }

    void Cohesion(size_t index) {
      if ( index >= m_Boids.size() ) return;

      Vec2 steeringForce;
      float totalForce = 0.0f;

      for ( size_t i = 0; i < m_Boids.size(); i++ ) {
        if ( i == index ) continue;
        Vec2 dist = m_Boids[index].Position - m_Boids[i].Position;
        if ( Vec2::LengthSqr(dist) < s_ViewRadiusSq ) {
          steeringForce += m_Boids[i].Position;
          totalForce++;
        }
      }

      if ( totalForce > 0.0f ) {
        steeringForce /= totalForce;
        steeringForce -= m_Boids[index].Position;
        m_Boids[index].Acceleration += steeringForce;
      }
    }

    void DrawDebug(size_t index) {
      if ( index >= m_Boids.size() ) return;

      Boid& b = m_Boids[index];
      ImmGFX::Ref().Color(1.0f, 0.0f, 0.0f, 1.0f);
      ImmGFX::Ref().DrawCircleWire(b.Position, s_ViewRadius, 16);
      ImmGFX::Ref().Color(0.0f, 1.0f, 0.0f, 1.0f);
      ImmGFX::Ref().DrawLine(b.Position, b.Position + b.Velocity);
      ImmGFX::Ref().Color(0.0f, 0.0f, 1.0f, 1.0f);
      ImmGFX::Ref().DrawLine(b.Position, b.Position + b.Acceleration);
    }

  private:
    std::vector<Boid> m_Boids;
  };

}

