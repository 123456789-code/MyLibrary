#pragma once
#include <string>
#include <vector>

template <int max_type = 0>
class fsm_state {
public:
	__readonly std::string name;
	__readonly int connects[max_type + 1];
		
	constexpr fsm_state(const fsm_state&)noexcept = default;
	constexpr fsm_state(fsm_state&&)noexcept = default;
	constexpr fsm_state(const std::string& name_) : name(name_) {  };
	constexpr fsm_state(std::string&& name_) : name(name_) {  };
	constexpr ~fsm_state() = default;

	constexpr void connect(const int& id, const int& input = 0) { connects[input] = id; }
};

template <int max_type = 0>
class fsm {
private:
	__readonly std::vector<fsm_state<max_type>> states;
	__readonly int id_now;

public:
	constexpr fsm()noexcept = default;
	constexpr fsm(const fsm&)noexcept = default;
	constexpr fsm(fsm&&)noexcept = default;
	constexpr ~fsm() = default;

	template<typename ... others>
	constexpr void append(const std::string& t, const others& ... o) {
		states.emplace_back(t);
		if constexpr (sizeof...(o) != 0) { append(o...); }
	}
	template<typename ... others>
	constexpr void append(std::string&& t, others&& ... o) {
		states.emplace_back(t);
		if constexpr (sizeof...(o) != 0) { append(o...); }
	}

	constexpr  int get_id(const std::string& name)const { for (int i = 0; i <= max_type; i++) if (states[i].name == name) return i; assert(0); }
	constexpr std::string get_name(const int& id)const { return states[id].name; }
	constexpr std::string state_now()const { return get_name(id_now); }
	constexpr void set_state(const std::string& name) { id_now = get_id(name); }

	constexpr void set_rule(const std::string& a, const std::string& b, const int& input = 0) { states[get_id(a)].connect(get_id(b), input); }
	constexpr void step(const int& input = 0) { id_now = states[id_now].connects[input]; }
};