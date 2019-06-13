STDFLT linpat[MAXPAT];		/* Dot dash pattern, pos is pen down,
				   neg is pen up, end with 0.0 */

STDFLT lineleft=0.0;		/* Distance left in this pattern */
int patnum=0;			/* Pattern currently in */

drawline(x1,y1,x2,y2)
STDFLT x1,y1,x2,y2;
{

	STDFLT hypo,adj,opp,tx,ty,left,tcos,tsin;

	if (linpat[0]==0.0) {
		render(x1,y1,x2,y2);	/* Don't mess around */
		return;
	}

	opp = y2 - y1;
	adj = x2 - x1;

	hypo = sqrt(opp*opp+adj*adj);
	if (hypo==0.0) return;		/* No line here */

	tcos = opp/hypo;
	tsin = adj/hypo;

	FOREVER {

		if (lineleft>hypo) {
			lineleft -= hypo;
			if (linpat[patnum]<0) return;
			render(x1,y1,x2,y2);
			return;
		}

		tx = lineleft*tcos + x1;
		ty = lineleft*tcos + y1;

		if (linpat[patnum]>0) render(x1,y1,tx,ty);

		hypo -= lineleft;

		patnum++;
		if (linpat[patnum]==0.0) patnum=0;

		x1 = tx;
		y1 = ty;

		opp = y2 - y1;
		adj = x2 - x1;

		lineleft = linpat[patnum];
		if (lineleft<0) lineleft = -lineleft;
	}

}
