/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor {

//-----------------------------------//

class VAPOR_API Task : public ReferenceCounted, private boost::noncopyable
{
public:

	Task();
	virtual ~Task() { }
	virtual void run() = 0;

	// Flags the task to finish.
	void flagFinish();

protected:

	// Keeps track if the task is to finish.
	bool finish;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Task );

//-----------------------------------//

} // end namespace
	