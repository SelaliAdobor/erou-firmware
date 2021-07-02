import asyncio
import websockets
import random
import traceback

uri = "ws://192.168.1.200:80/debug"
containerCount = 8
iterationDelay = 10
success = 0
failures = 0
successes_since_last_failure = 0


async def run():
    global success
    global failures
    global ran_since_failure
    global successes_since_last_failure
    while True:
        try:
            async with websockets.connect(uri) as websocket:
                while True:
                    await asyncio.sleep(iterationDelay)
                    current_command = f'gtc {random.randint(0, containerCount - 1)}'
                    print(f'Sending:  {current_command}')
                    await websocket.send(current_command)
                    while True:
                        received = await websocket.recv()
                        print(f'Received:  {received.rstrip()}')
                        if "Went to container" in received:
                            break
                    success += 1
                    successes_since_last_failure += 1
                    print(
                        f'Went to container {successes_since_last_failure} time(s) since last restart, {success} '
                        f'successful trip(s) in total, restarted {failures} times(s)')
        except Exception as ex:
            print(
                f'An exception {ex.__class__.__name__} occurred after {successes_since_last_failure} time(s) since last restart, {success} '
                f'trip(s) in total, restarted {failures} times(s) before this')
            if successes_since_last_failure > 0:
                failures += 1
                successes_since_last_failure = 0
            else:
                print(f'Ignoring repeated failure')


asyncio.get_event_loop().run_until_complete(run())
