#pragma once

#include "IStresserDetector.h"
#include "IArtifactSubscriber.h"

const int STRESSER_DRIVER_RESOURCE_ID = 5;
const std::wstring STRESSER_DRIVER_RESOURCE_NAME(L"STRESSER_ENGINE");
const std::wstring STRESSER_DRIVER_SERVICE_NAME(L"StresserProcessDetector");
const std::wstring STRESSER_DRIVER_NAME_WITH_EXTENSION(L"StresserEngine.sys");
const std::uint32_t STRESSER_DRIVER_SERVICE_TYPE = SERVICE_KERNEL_DRIVER;

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
	void setNewArtifacts(const std::vector<IArtifact*>& artifacts) override;

private:
	/* Extract driver from PE resources and dump it to disk, create Win32 service and start it. */
	static void installStresserDriver();

	/* Stop the service and delete the driver binary from the disk. */
	static void uninstallStresserDriver();

	/* Register available process ID for detection. */
	void registerFakeProcessIds();

	static std::wstring createTemporaryPath(const std::wstring& exeNameWithExtension);

	bool m_doesTestSigning;
};
