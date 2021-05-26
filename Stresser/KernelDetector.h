#pragma once

#include "IStresserDetector.h"
#include "IArtifactSubscriber.h"
#include "ProcessDetector.h"

#include "../Utils/StandardThread.h"
#include "../Utils/WindowsEvent.h"

const int STRESSER_DRIVER_RESOURCE_ID = 5;
const std::wstring STRESSER_DRIVER_RESOURCE_NAME(L"STRESSER_ENGINE");
const std::wstring STRESSER_DRIVER_SERVICE_NAME(L"StresserProcessDetector");
const std::wstring STRESSER_DRIVER_NAME_WITH_EXTENSION(L"StresserEngine.sys");
const std::uint32_t STRESSER_DRIVER_SERVICE_TYPE = SERVICE_KERNEL_DRIVER;
const std::wstring STOP_FETCH_THREAD_EVENT_NAME(L"StopFetchProcessEvents");
const int WAIT_BETWEEN_FETCH_EVENT_MS = 20000;
const std::wstring TIME_FORMAT(L"%d/%m/%Y - %H:%M:%S");
const std::wstring ON_FAKE_PROCESS_EVENT_NAME(L"OnFakeProcess");
const std::wstring UM_ON_FAKE_PROCESS_EVENT_NAME(L"Global\\" + ON_FAKE_PROCESS_EVENT_NAME);
const std::wstring KM_ON_FAKE_PROCESS_EVENT_NAME(L"\\BaseNamedObjects\\" + ON_FAKE_PROCESS_EVENT_NAME);

class KernelDetector : public IStresserDetector, public IArtifactSubscriber
{
public:
	explicit KernelDetector(const EventController& eventController);

	// Disable: copyable, assignable, movable:
	KernelDetector(KernelDetector&) = delete;
	KernelDetector& operator=(KernelDetector&) = delete;
	KernelDetector(KernelDetector&&) = delete;
	KernelDetector& operator=(KernelDetector&&) = delete;

	virtual ~KernelDetector();

	/*
		Start the detector.
		If encounter any error, log it and keep running Stresser without terminating it.
	*/
	void start() override;

	/* Stop the detector */
	void stop() override;

	/*
		Called by the ArtifactManager whenever new Artifacts arrived.
		Used to stop flush the kernel detector fake process IDs and start
		detection on new ones.
	 */
	void setNewArtifacts(const std::vector<std::shared_ptr<IArtifact>>& artifacts) override;

private:
	/* Extract driver from PE resources and dump it to disk, create Win32 service and start it. */
	static void installStresserDriver();

	/* Stop the service and delete the driver binary from the disk. */
	static void uninstallStresserDriver();

	/* Register available process ID for detection. */
	void registerFakeProcessIds();

	/* Request to remove detection of all the registered process IDs. */
	static void removeAllRegisteredFakeProcessIds();

	/* Helper for concatenating temporary path with filename. */
	static std::wstring createTemporaryPath(const std::wstring& exeNameWithExtension);

	/*
		Represent the detection thread.
		Try to consume available events or wait for the kernel detector to signal
		on new arrived events to be consumed.
	*/
	static void fetchAndSendEvents(LPVOID params);

	/* Handler the received events. */
	static void onProcessDetectionEvent(const EventsResult& eventsResult);

	/* Reset stop event and start the detection thread. */
	void startDetectionThread();

	/* Set the stop event and stop the detection thread. */
	void stopDetectionThread();

	bool m_doesTestSigning;
	StandardThread m_fetchEventThread;
	WindowsEvent m_onProcessEvent;
	WindowsEvent m_stopDetectionThreadEvent;
};
