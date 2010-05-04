
local tests = {}
local currentTest = nil

function testFailed(reason)
  print(" *** FAILURE ***")
  if (reason) then
    print(reason)
  else
    print("Unknown reason")
  end
end

function testSuccess()
  print(" *** SUCCESS ***")
  timer.Simple(0, nextTest)
end

function nextTest()
  // find the next test to run
  currentTest = next(tests, currentTest)
  if (!currentTest) then
    print(" *** FINISHED RUNNING TESTS ***")
    return;
  end
  
  local currentTestInfo = tests[currentTest]
  
  // print the test name and run the function
  print("==[ " .. currentTestInfo.name .. " ]==============================")
  local status, result = pcall( currentTestInfo.test )
  if (!status) then
    // somekind of error, print it
    testFailed(result)
  end
end

function addTest(_name, _test)
  table.insert( tests, {name=_name, test = _test } );
end

timer.Simple(0, nextTest)

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Module test code goes here!
////////////////////////////////////////////////////////////////////////////////////////////////////////

require("mysqloo")

local DATABASE_HOST = "localhost"
local DATABASE_PORT = 3308
local DATABASE_NAME = "test"
local DATABASE_USERNAME = "root"
local DATABASE_PASSWORD = "1234"

local databaseObject = nil

addTest("Connect", function()
  databaseObject = mysqloo.connect(DATABASE_HOST, DATABASE_USERNAME, DATABASE_PASSWORD, DATABASE_NAME, DATABASE_PORT)
  print(databaseObject) // Should print "[Database:<address>]"
  
  function databaseObject.onConnected(self)
    testSuccess()
  end
  
  function databaseObject.onConnectionFailed(self, error)
    testFailed(error)
  end
  
  databaseObject:connect()

  return true
end )

addTest("Escape", function()
  local test = "a \r b \n c \" "; 
  local escapedString = databaseObject:escape(test)
  print(escapedString)
  testSuccess()
end )

addTest("SimpleQuery", function()
  local simpleQuery = databaseObject:query("SELECT 5 + 5")
  function simpleQuery.onSuccess(self)
    PrintTable(self:getData())
    testSuccess()
  end
  function simpleQuery.onError(self, error)
    testFailed(error)
  end
  simpleQuery:start()
end )
/*
addTest("SingleInsert", function()
  local insertQuery = databaseObject:query("INSERT INTO test (name, cost) VALUES('Can of Coke', '0.75' )")
  function insertQuery.onSuccess(self)
    print("inserted id = " .. self:lastInsert())
    testSuccess()
  end
  function insertQuery.onError(self, error)
    testFailed(error)
  end
  insertQuery:start()
end )

addTest("MultipleInsert", function()
  local insertQuery = databaseObject:query("INSERT INTO test (name, cost) VALUES ('Mars bar', '0.50' ),('Red wine', '5.79' )")
  function insertQuery.onSuccess(self)
    print("inserted id = " .. self:lastInsert())
    print("number of rows = " .. self:affectedRows())
    testSuccess()
  end
  function insertQuery.onError(self, error)
    testFailed(error)
  end
  insertQuery:start()
end )
*/
addTest("TableQuery - Named", function()
  local selectQuery = databaseObject:query("SELECT ID, Name, Cost FROM test WHERE Cost > 0.50")
  function selectQuery.onData(self, data)
    print( data.ID, data.Name, data.Cost )
  end
  function selectQuery.onSuccess(self)
    testSuccess()
  end
  function selectQuery.onError(self, error)
    testFailed(error)
  end
  selectQuery:start()
end )

addTest("TableQuery - Functions", function()
  local selectQuery = databaseObject:query("SELECT AVG(Cost),MIN(Cost),MAX(Cost),COUNT(Cost) FROM test")
  function selectQuery.onData(self, data)
    print("Cheapest Item: ", data["MIN(Cost)"])
    print("Most Expensive Item: ", data["MAX(Cost)"])
    print("Number of items: ", data["COUNT(Cost)"])
  end
  function selectQuery.onSuccess(self)
    testSuccess()
  end
  function selectQuery.onError(self, error)
    testFailed(error)
  end
  selectQuery:setOption( mysqloo.OPTION_CACHE, false);
  selectQuery:start()
end )

addTest("TableQuery - Indexed", function()
  local selectQuery = databaseObject:query("SELECT ID, Name, Cost FROM test WHERE Cost > 0.50")
  function selectQuery.onData(self, data)
    print( data[1], data[2], data[3] )
  end
  function selectQuery.onSuccess(self)
    testSuccess()
  end
  function selectQuery.onError(self, error)
    testFailed(error)
  end
  selectQuery:setOption( mysqloo.OPTION_NUMERIC_FIELDS );
  selectQuery:setOption( mysqloo.OPTION_NAMED_FIELDS, false);
  selectQuery:start()
end )

addTest("TableQuery - Abort", function()
  local selectQuery = databaseObject:query("SELECT ID, Name, Cost FROM test")
  function selectQuery.onData(self, data)
    print( data[1], data[2], data[3] )
  end
  function selectQuery.onAborted(self)
    testSuccess()
  end
  function selectQuery.onSuccess(self)
    testFailed(error)
  end
  function selectQuery.onError(self, error)
    testFailed(error)
  end
  selectQuery:abort() // ensure the query aborts straight away
  selectQuery:start()
end )

queriesComplete = 0

addTest("TableQuery - 2 at once", function()
  queriesComplete = 0
  
  function onSuccess()
    queriesComplete = queriesComplete + 1
    if (queriesComplete == 2) then
      testSuccess(error)
     end  
  end
  
  local selectQuery1 = databaseObject:query("SELECT ID, Name, Cost FROM test")
  selectQuery1.onSuccess = onSuccess
  function selectQuery1.onData(self, data)
    print( "selectQuery1", data.ID, data.Name, data.Cost )
  end
  function selectQuery1.onError(self, error)
    testFailed(error)
  end

  local selectQuery2 = databaseObject:query("SELECT ID, Name, Cost FROM test")
  selectQuery2.onSuccess = onSuccess
  function selectQuery2.onData(self, data)
    print( "selectQuery2", data.ID, data.Name, data.Cost )
  end
  function selectQuery2.onError(self, error)
    testFailed(error)
  end
  print("selectQuery2 started")
  selectQuery2:start()
  print("selectQuery1 started")
  selectQuery1:start()
end )
