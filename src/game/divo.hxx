/**
 * @file divo.hxx
 * @desc Divo header.
 */
#ifndef GAME_DIVO_HXX
#define GAME_DIVO_HXX

#include "movable.hxx"

 /**
  * A divo class.
  */
class Divo : public Movable
{
public:
	/**
	 * Destructs the divo.
	 */
	~Divo();

	/**
	 * Constructs the divo.
	 */
	Divo();

	void setId(int divoId);
	bool idle() const { return !walking; }
	virtual void nextMove();
	virtual void setMap(Map *map);

private:
	Divo(const Divo&);
	Divo& operator=(const Divo&);
};

#endif // GAME_DIVO_HXX

