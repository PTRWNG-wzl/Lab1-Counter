module counter #(
    parameter width = 8
)(
    // interface signals
    input logic clk,
    input logic rst, 
    input logic en,
    output logic [width-1:0] count
);

always_ff @(posedge clk) 
begin
    if (rst)
        count <= 0;
    else
        count <= count + 1;
end

endmodule
