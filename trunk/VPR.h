#ifndef VPR_H
#define VPR_H

#include "PR.h"

NAMESPACE_PR_BEGIN
/**
 * \brief "Volumetric"
 * 
 * It will
 * - Read an external point cloud.
 * - Convert into level set signed distance function
 * - Fill the inner cell with attributes.
 * - Write to external .ptc file and convert it to .bkm file.
 * - You can use this .bkm file in you shader.
 * 
 * Go to <a href="http://software.primefocusworld.com/software/support/krakatoa/render_geometry_volumes.php">CONVERTING GEOMETRY VOLUMES TO PARTICLE CLOUDS</a> for more details.
 */
class VolumetricParticleSolver : public ParticleResolver
{
public:
	VolumetricParticleSolver();
	~VolumetricParticleSolver();

	/** \brief Setup the resolution of volume, the greater the better, but the memory and speed should be considered.
	 */
	void SetRes(const RtInt&, const RtInt&, const RtInt&);

	/** \brief Assign the .bkm path.
	 */
	void SetBKMPath(const char*);
	
	/**
	 * \warning It will not call RiPointsV.
	 */
	RtVoid DoIt(RtInt, RtInt, RtToken [], RtPointer []);
private:
	RtInt mRes[3];
	std::string mBKMPath;
};

NAMESPACE_PR_END

#endif
