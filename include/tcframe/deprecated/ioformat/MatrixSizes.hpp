#pragma once

namespace tcframe_old {

class MatrixSizes {
public:
    MatrixSizes(int rowsSize, int columnsSize)
            : rowsSize(rowsSize), columnsSize(columnsSize) { }

    int getRowsSize() {
        return rowsSize;
    }

    int getColumnsize() {
        return columnsSize;
    }

private:
    int rowsSize;
    int columnsSize;
};

}
