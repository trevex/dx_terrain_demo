/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 *
 * Unfortunately there is no template work-around or pointer trick to get loads of states working properly
 * in a single statemanager (with a performance benefit), so statics are used to avoid virtuals (vtables) and get a small performance gain
 * [ dynamic dispatching http://en.wikipedia.org/wiki/Dynamic_dispatch ]
 *
 * the old code is left for later inspection why it did not work
 */

#ifndef STATEMANAGER_H_
#define STATEMANAGER_H_

/*
#include <functional>

template <typename StateClass, typename ReturnType, typename Arg>
struct bindStateFunction { 
    bindStateFunction(std::mem_fun1_t<ReturnType, StateClass, Arg> f, StateClass* state) : func(f), pState(state) {} 

    ReturnType operator()(Arg arg) 
	{ 
		return func(pState, arg); 
	} 

    std::mem_fun1_t<ReturnType, StateClass, Arg> func; 
    StateClass* pState; 
};

typedef bool (*updateFunc)(float delta);

template<typename Type, class Func> Type make_function_pointer(Func& f) {
    return lambda_wrapper<Type, Func>::get_function_pointer(f);
}

template<typename T, class F> class lambda_wrapper {
    static F* func_;
    static bool func (float delta) { return (*func_)(delta); }    
    friend T make_function_pointer<>(F& f);    
    static T get_function_pointer(F& f) {
        if (!func_) func_ = new F(f);
        return func;
    }
};

template<typename T, class F> F* lambda_wrapper<T, F>::func_ = 0;

class StateManager
{
public:
	template <typename StateClass> static void setState(StateClass* nextState)
	{
		static bindStateFunction<StateClass, bool, float> functorUpdate(std::mem_fun1(&StateClass::update), nextState);
		//m_funcptrUpdate = make_function_pointer<funcptrUpdateType>([&]( float t)->bool { return functorUpdate(t); });
	}
	static bool update(float delta) 
	{
		return m_funcUpdate(delta);
	}
private:
	typedef bool (*funcUpdateType)(float);
	static funcUpdateType m_funcUpdate;
};*/


class StateManager
{
public:
	template <typename S> static void set(void* options = 0)
	{
		// leave old state
		if (m_funcLeave) m_funcLeave();

		// set function pointers
		m_funcUpdate = S::update;
		m_funcLeave = S::leave;
		m_funcGeom = S::renderGeometry;
		m_funcLight = S::renderLight;
		m_funcPost = S::renderPost;
		m_funcForward = S::renderForward;

		// enter new state
		S::enter(options);
	}
	static bool update(const float &delta);
	static void renderGeometry(void);
	static void renderLight(void);
	static void renderForward(void);
	static void renderPost(void);
protected:
	typedef bool (*funcUpdateType)(const float &delta);
	typedef bool (*funcLeaveType)(void);
	typedef void (*funcRenderType)(void);
private:	
	static funcUpdateType m_funcUpdate;
	static funcLeaveType m_funcLeave;
	static funcRenderType m_funcGeom;
	static funcRenderType m_funcLight;
	static funcRenderType m_funcPost;
	static funcRenderType m_funcForward;
};

#endif