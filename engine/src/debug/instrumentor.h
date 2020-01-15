#pragma once

// https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Debug/Instrumentor.h

#include <string>
#include <algorithm>
#include <fstream>
#include <thread>
#include <stack>
#include <list>

#include "../common/types.h"
#include "../core/timer.h"

namespace prev {

	struct ProfileResult {
		const char * Name;
		pvint64 Start, End;
		pvuint ThreadID;
	};

	struct InstrumentationSession {
		std::string Name;
	};

	class Instrumentor {
	private:
		InstrumentationSession * m_CurrentSession;
		std::ofstream m_OutputStream;
		int m_ProfileCount;
	public:
		Instrumentor()
			: m_CurrentSession(nullptr), m_ProfileCount(0) {
		}

		void BeginSession(const std::string & name, const std::string & filepath = "results.json") {
			m_OutputStream.open(filepath);
			WriteHeader();
			m_CurrentSession = new InstrumentationSession{ name };
		}

		void EndSession() {
			WriteFooter();
			m_OutputStream.close();
			delete m_CurrentSession;
			m_CurrentSession = nullptr;
			m_ProfileCount = 0;
		}

		void WriteProfile(const ProfileResult & result) {
			if (m_ProfileCount++ > 0)
				m_OutputStream << ",";

			std::string name = result.Name;
			std::replace(name.begin(), name.end(), '"', '\'');

			m_OutputStream << "{";
			m_OutputStream << "\"cat\":\"function\",";
			m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
			m_OutputStream << "\"name\":\"" << name << "\",";
			m_OutputStream << "\"ph\":\"X\",";
			m_OutputStream << "\"pid\":0,";
			m_OutputStream << "\"tid\":" << result.ThreadID << ",";
			m_OutputStream << "\"ts\":" << result.Start;
			m_OutputStream << "}";

			m_OutputStream.flush();
		}

		void WriteHeader() {
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
			m_OutputStream.flush();
		}

		void WriteFooter() {
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		static Instrumentor & Get() {
			static Instrumentor instance;
			return instance;
		}
	};

	class InstrumentationTimer {
	public:
		InstrumentationTimer(const char * name)
			: m_Name(name), m_Stopped(false) {

			m_StartTimepoint = static_cast<pvint64>(Timer::GetTime().GetNS());
			if (!s_ParentsTimes.empty()) {
				if (m_StartTimepoint == s_ParentsTimes.top()) {
					m_StartTimepoint += 1;
				}
			}
			s_ParentsTimes.push(m_StartTimepoint);
		}

		~InstrumentationTimer() {
			if (!m_Stopped)
				Stop();
			s_ParentsTimes.pop();
			if (s_ParentsTimes.empty()) {
				for (pvuint i = 0; i < s_ArrayIndex; i++) {
					Instrumentor::Get().WriteProfile(s_Results[i]);
				}
				s_ArrayIndex = 0u;
			}
		}

		void Stop() {
			auto endTimepoint = static_cast<pvint64>(Timer::GetTime().GetNS());
			uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
			s_Results[s_ArrayIndex].Name = m_Name;
			s_Results[s_ArrayIndex].Start = m_StartTimepoint;
			s_Results[s_ArrayIndex].End = endTimepoint;
			s_Results[s_ArrayIndex].ThreadID = threadID;
			s_ArrayIndex++;
			m_Stopped = true;
		}
	private:
		static std::stack<pvint64> s_ParentsTimes;
		static ProfileResult s_Results[512];
		static pvuint s_ArrayIndex;

		const char * m_Name;
		pvint64 m_StartTimepoint;
		bool m_Stopped;
	};

}

#ifdef ENGINE_PROFILE
	#define PV_PROFILE_BEGIN_SESSION(name, filepath) ::prev::Instrumentor::Get().BeginSession(name, filepath)
	#define PV_PROFILE_END_SESSION() ::prev::Instrumentor::Get().EndSession()
#define PV_PROFILE_SCOPE(name) ::prev::InstrumentationTimer timer##__LINE__(name);
	#if defined(ENGINE_WINDOWS)
		#define PV_PROFILE_FUNCTION() PV_PROFILE_SCOPE(__FUNCSIG__)
	#elif defined(ENGINE_LINUX)
		#define PV_PROFILE_FUNCTION() PV_PROFILE_SCOPE(__PRETTY_FUNCTION__)
	#endif
#else
	#define PV_PROFILE_BEGIN_SESSION(name, filepath)
	#define PV_PROFILE_END_SESSION()
	#define PV_PROFILE_SCOPE(name)
	#define PV_PROFILE_FUNCTION()
#endif