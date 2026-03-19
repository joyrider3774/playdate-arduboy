
/**
 * @brief Manage game state
 *
 */
template <class STATE, uint8_t MS_PER_FRAME>
class StateController {
  public:
    STATE state;
    STATE next_state;
    uint16_t next_time = 0;
    bool just_entered = true;

    void frame() {
      if (next_time > 0) {
        if (next_time <= MS_PER_FRAME) {
          next_time = 0;
          state = next_state;
          just_entered = true;
        } else {
          next_time -= MS_PER_FRAME;
          just_entered = false;
        }
      }
    }

    void go(STATE next, uint16_t time=1) {
      next_state = next;
      next_time = time;
    }

    bool entered(STATE check_state) {
      return (check_state == state && just_entered);
    }

    bool in(STATE a) {
      return state == a;
    }

    bool in(STATE a, STATE b) {
      return in(a) || in(b);
    }

    bool in(STATE a, STATE b, STATE c) {
      return in(a) || in(b) || in(c);
    }
};
