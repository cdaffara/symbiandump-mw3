/*------------------------------------------------------------------------------
* Copyright (C) 2003-2006 Ben van Klinken and the CLucene Team
* 
* Distributable under the terms of either the Apache License (Version 2.0) or 
* the GNU Lesser General Public License, as specified in the COPYING file.
------------------------------------------------------------------------------*/
#ifndef _lucene_search_TermQuery_
#define _lucene_search_TermQuery_

#if defined(_LUCENE_PRAGMA_ONCE)
# pragma once
#endif

#include "clucene/search/searchheader.h"
#include "clucene/search/scorer.h"
#include "clucene/index/term.h"
#include "clucene/search/termscorer.h"
#include "clucene/index/indexreader.h"
#include "clucene/util/stringbuffer.h"
#include "clucene/index/terms.h"

CL_NS_DEF(search)


    /** A Query that matches documents containing a term.
	This may be combined with other terms with a {@link BooleanQuery}.
	*/
    class TermQuery: public Query {
    private:
		CL_NS(index)::Term* term;

		
		class TermWeight: public Weight {
		private:
			Searcher* searcher;
			float_t value;
			float_t idf;
			float_t queryNorm;
			float_t queryWeight;
			TermQuery* _this;
			CL_NS(index)::Term* _term;

		public:
			TermWeight(Searcher* searcher, TermQuery* _this, CL_NS(index)::Term* _term);
			~TermWeight();
			TCHAR* toString();
			Query* getQuery() { return (Query*)_this; }
			float_t getValue() { return value; }

			float_t sumOfSquaredWeights();
			void normalize(float_t queryNorm);
			Scorer* scorer(CL_NS(index)::IndexReader* reader);
			void explain(CL_NS(index)::IndexReader* reader, int32_t doc, Explanation* ret);
		};

    protected:
        Weight* _createWeight(Searcher* searcher);
        TermQuery(const TermQuery& clone);
	public:
		// Constructs a query for the term <code>t</code>. 
		TermQuery(CL_NS(index)::Term* t);
		~TermQuery();

		static const TCHAR* getClassName();
		const TCHAR* getQueryName() const;
	    
		//added by search highlighter
		CL_NS(index)::Term* getTerm(bool pointer=true) const;
	    
		// Prints a user-readable version of this query. 
		TCHAR* toString(const TCHAR* field) const;

		bool equals(Query* other) const;
		Query* clone() const;

		/** Returns a hash code value for this object.*/
		size_t hashCode() const;
    };
CL_NS_END
#endif

