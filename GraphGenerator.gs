function importCSV() {
  file = DriveApp.getFilesByName("log.csv");
  if (!file.hasNext()) {
    return -1;
  }
  csvFile = file.next();

  const csvText = csvFile.getBlob().getDataAsString();
  const csvData = Utilities.parseCsv(csvText);
  csvFile.setTrashed(true);

  const sheets = SpreadsheetApp.getActiveSpreadsheet();
  const dataSheet = sheets.getSheetByName("Data");
  dataSheet.getRange("B" + (dataSheet.getLastRow() + 1) + ":E" + (dataSheet.getLastRow() + csvData.length)).setValues(csvData);
}

function updateGraph() {
  const sheets = SpreadsheetApp.getActiveSpreadsheet();
  const graphSheet = sheets.getSheetByName("Graph");
  const dataSheet = sheets.getSheetByName("Data");

  const firstGetDate = graphSheet.getRange("B3").getValue();
  if (Object.prototype.toString.call(firstGetDate) != "[object Date]") {
    Browser.msgBox("初回取得日時の値が不正です", Browser.Buttons.OK);
    return -1;
  }
  var defaultDate = graphSheet.getRange("C3");
  if (defaultDate.isBlank()) {
    firstGetDate.setMinutes(Math.floor(firstGetDate.getMinutes() / 10) * 10);
    firstGetDate.setMinutes(firstGetDate.getMinutes() - (10 * (dataSheet.getLastRow() - 2)));
    defaultDate.setValue(firstGetDate);
  }
  defaultDate = defaultDate.getValue();
  const startDate = graphSheet.getRange("D3").getValue();
  const endDate = graphSheet.getRange("E3").getValue();

  dataSheet.unhideRow(dataSheet.getRange("A1:A"));
  importCSV();
  dataSheet.getRange("A2:A" + dataSheet.getLastRow()).clearContent();
  dataSheet.getRange("A2").setValue(defaultDate);
  defaultDate.setMinutes(defaultDate.getMinutes() + 10);
  dataSheet.getRange("A3").setValue(defaultDate);
  dataSheet.getRange("A2:A3").autoFillToNeighbor(SpreadsheetApp.AutoFillSeries.DEFAULT_SERIES);

  const startRow = findRow(dataSheet, startDate);
  if (startRow == -1) {
    Browser.msgBox("表示開始日の値がデータ範囲外です", Browser.Buttons.OK);
    return -1;
  }
  var endRow = findRow(dataSheet, endDate);
  if (endRow == -1) {
    Browser.msgBox("表示終了日の値がデータ範囲外です", Browser.Buttons.OK);
    return -1;
  }
  endDate.setDate(endDate.getDate() + 1);
  const endNextRow = findRow(dataSheet, endDate);
  if (endNextRow == -1) {
    endRow = dataSheet.getLastRow();
  } else {
    endRow = endNextRow - 1;
  }

  if (startRow != 2) {
    dataSheet.hideRow(dataSheet.getRange("A2:A" + (startRow - 1)));
  }

  for (var i = 0; i < 4; i++) {
    updateChartRange(graphSheet, dataSheet, i, endRow);
  }
}

function findRow(sheet, date) {
  const values = sheet.getDataRange().getValues();
  for (var i = 1; i < values.length; i++) {
    if (String(Utilities.formatDate(values[i][0], "JST", "yyyy-MM-dd")) == String(Utilities.formatDate(date, "JST", "yyyy-MM-dd"))) {
      return i + 1;
    }
  }
  return -1;
}

function updateChartRange(graphSheet, dataSheet, number, endRow) {
  const chart = graphSheet.getCharts()[number];
  const cells = ["B", "C", "D", "E"];
  const newChart = chart
    .modify()
    .clearRanges()
    .addRange(dataSheet.getRange("A1:A" + endRow))
    .addRange(dataSheet.getRange(cells[number] + "1:" + cells[number] + endRow))
    .build();
  graphSheet.updateChart(newChart);
}
