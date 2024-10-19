// function doGet(e) { 
//   Logger.log(JSON.stringify(e));
//   var result = 'Ok';

//   // Kiểm tra nếu thiếu sheet_id hoặc sheet_name
//   if (!e.parameter.sheet_id || !e.parameter.sheet_name) {
//     result = 'Missing sheet_id or sheet_name';
//     return ContentService.createTextOutput(result);
//   }

//   var sheet_id = e.parameter.sheet_id;  // Lấy sheet_id từ tham số
//   var sheet_name = e.parameter.sheet_name;  // Lấy sheet_name từ tham số

//   var sheet_open = SpreadsheetApp.openById(sheet_id);
//   var sheet_target = sheet_open.getSheetByName(sheet_name);

//   var newRow = sheet_target.getLastRow() + 1;

//   var rowDataLog = [];
//   var Data_for_I3, Data_for_J3, Data_for_K3, Data_for_L3, Data_for_M3, Data_for_N3, Data_for_O3;

//   var Curr_Date = Utilities.formatDate(new Date(), "Asia/Jakarta", 'dd/MM/yyyy');
//   rowDataLog[0] = Curr_Date;
//   Data_for_I3 = Curr_Date;

//   var Curr_Time = Utilities.formatDate(new Date(), "Asia/Jakarta", 'HH:mm:ss');
//   rowDataLog[1] = Curr_Time;
//   Data_for_J3 = Curr_Time;

//   var sts_val = '';

//   for (var param in e.parameter) {
//     var value = stripQuotes(e.parameter[param]);
//     switch (param) {
//       case 'sts':
//         sts_val = value;
//         break;

//       case 'srs':
//         rowDataLog[2] = value;
//         Data_for_K3 = value;
//         break;

//       case 'temp':
//         rowDataLog[3] = value;
//         Data_for_L3 = value;
//         break;

//       case 'humd':
//         rowDataLog[4] = value;
//         Data_for_M3 = value;
//         break;

//       case 'swtc1':
//         rowDataLog[5] = value;
//         Data_for_N3 = value;
//         break;

//       case 'swtc2':
//         rowDataLog[6] = value;
//         Data_for_O3 = value;
//         break;

//       default:
//         result += ", unsupported parameter";
//     }
//   }

//   if (sts_val == 'write') {
//     var newRangeDataLog = sheet_target.getRange(newRow, 1, 1, rowDataLog.length);
//     newRangeDataLog.setValues([rowDataLog]);

//     var RangeDataLatest = sheet_target.getRange('I3:O3');
//     RangeDataLatest.setValues([[Data_for_I3, Data_for_J3, Data_for_K3, Data_for_L3, Data_for_M3, Data_for_N3, Data_for_O3]]);

//     return ContentService.createTextOutput(result);
//   }

//   if (sts_val == 'read') {
//     var all_Data = sheet_target.getRange('K3:O3').getValues();
//     return ContentService.createTextOutput(all_Data);
//   }
// }

// function stripQuotes(value) {
//   return value.replace(/^["']|['"]$/g, "");
// }
