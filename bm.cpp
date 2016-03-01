/*

Original Copyright 2016 Vlad Didenko

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

#include <array>
#include <deque>
#include <cstdlib>
#include "benchmark/benchmark.h"
#include "boost/lockfree/queue.hpp"

namespace didenko {
    namespace bm {

        static void dequeSTnarrow( benchmark::State &state ) {
            const auto barrier = (RAND_MAX / 3) * 2;
            std::deque<int> queue{ };

            srand( time( NULL ));

            while ( state.KeepRunning()) {
                state.PauseTiming();
                auto r = rand();
                if ( r > barrier ) {
                    state.ResumeTiming();
                    queue.push_front( r );
                } else {
                    if ( queue.size() > 0 ) {
                        state.ResumeTiming();
                        queue.pop_back();
                    }
                }
            }
            state.SetItemsProcessed( state.iterations());
        }

        static void dequeSTfull( benchmark::State &state ) {
            const auto barrier = (RAND_MAX / 3) * 2;
            std::deque<int> queue{ };

            srand( time( NULL ));

            while ( state.KeepRunning()) {
                auto r = rand();
                if ( r > barrier ) {
                    queue.push_front( r );
                } else {
                    if ( queue.size() > 0 ) {
                        queue.pop_back();
                    }
                }
            }
            state.SetItemsProcessed( state.iterations());
        }

        static void lckfreeMTnarrow( benchmark::State &state ) {
            const auto barrier = (RAND_MAX / 3) * 2;
            int        *iptr;
            int        r;

            boost::lockfree::queue<int *> q;

            srand( time( NULL ));

            if ( state.thread_index == 0 ) { ; // Setup code here
                iptr = &r;
            }

            while ( state.KeepRunning()) {
                state.PauseTiming();
                r = rand();
                if ( r > barrier ) {
                    state.ResumeTiming();
                    q.push( iptr );
                } else {
                    state.ResumeTiming();
                    if ( !q.empty()) q.pop( iptr );
                }
            }
            state.SetItemsProcessed( state.iterations());

            if ( state.thread_index == 0 ) { ; // Teardown code here.
            }
        }

        static void lckfreeMTfull( benchmark::State &state ) {
            const auto barrier = (RAND_MAX / 3) * 2;
            int        *iptr;
            int        r;

            boost::lockfree::queue<int *> q;

            srand( time( NULL ));

            if ( state.thread_index == 0 ) { ; // Setup code here
                iptr = &r;
            }

            while ( state.KeepRunning()) {
                r = rand();
                if ( r > barrier ) {
                    q.push( iptr );
                } else {
                    if ( !q.empty()) q.pop( iptr );
                }
            }
            state.SetItemsProcessed( state.iterations());

            if ( state.thread_index == 0 ) { ; // Teardown code here.
            }
        }

        BENCHMARK( lckfreeMTnarrow )->Threads( 1 );
        BENCHMARK( lckfreeMTfull )  ->Threads( 1 );

        BENCHMARK( lckfreeMTnarrow )->Threads( 1 )->UseRealTime();
        BENCHMARK( lckfreeMTfull )  ->Threads( 1 )->UseRealTime();

        BENCHMARK( lckfreeMTnarrow )->Threads( 5 );
        BENCHMARK( lckfreeMTfull )  ->Threads( 5 );

        BENCHMARK( lckfreeMTnarrow )->Threads( 5 )->UseRealTime();
        BENCHMARK( lckfreeMTfull )  ->Threads( 5 )->UseRealTime();

        BENCHMARK( dequeSTnarrow );
        BENCHMARK( dequeSTfull );
    }
}

BENCHMARK_MAIN();
